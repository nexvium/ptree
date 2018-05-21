// This file implementsd a class that encapsulates a list of all the processes
// that are running when the class is instantiated.
#include "Procs.hpp"
#include "ReadDir.hpp"

#include <stdexcept>
#include <cstdlib>
#include <fstream>
#include <sstream>

Procs::Procs()
{
  // get a list of all of the running pids from the /proc dir
  //
  const char proc_path[] = "/proc";
  ReadDir proc_dir(proc_path);
  while ( dirent* de = proc_dir.ReadNext() ) {
    if ( de->d_name[0] >= '0' && de->d_name[0] <= '9' ) {
      pid_t pid = atoi(de->d_name);
      ProcPtr proc = new Proc;
      proc->pid = pid;
      _procs.insert( pid_pair(pid, proc) );
    }
  }

  // fill in the rest of the process information from the 
  // individual /proc/<pid>/stat files
  auto itr = _procs.begin();
  while ( itr != _procs.end() ) {
    ProcPtr proc = itr->second;
    std::stringstream path;
    path << proc_path << "/" << proc->pid << "/" << "stat";

    std::fstream file(path.str(), std::fstream::in);
    if ( file.fail() ) {
      // XXX: Use post-increment to move itr to next element BEFORE erasing
      //      the current one so that the iterator remains valid.
      _procs.erase(itr++);
      continue;
    }

    // FIXME: the indexes of these fields should come from some kernel header
    string field;
    file >> field; //pid
    file >> proc->comm;
    file >> field; //state
    file >> proc->ppid;

    ++itr;
  }

  // get command lines from /proc/$$/cmdline
  itr = _procs.begin();
  while ( itr != _procs.end() ) {
    ProcPtr proc = itr->second;
    std::stringstream path;
    path << proc_path << "/" << proc->pid << "/" << "cmdline";

    std::fstream file(path.str(), std::fstream::in);
    if ( file.fail() ) {
      _procs.erase(itr++);
    }
    else {
      file >> proc->cmdline;
      if ( proc->cmdline.empty() )
        proc->cmdline = proc->comm;
      ++itr;
    }
  }

  // now that everything has been read, post-process to
  // 1) replace 0's in command line with spaces, and
  // 2) populate the children lists
  itr = _procs.begin();
  while ( itr != _procs.end() ) {
    ProcPtr proc = itr->second;

    size_t pos = 0;
    while ((pos = proc->cmdline.find((char)0, pos) ) && pos != string::npos)
      proc->cmdline.replace(pos, 1, " ");

    if (proc->ppid != 0)
      _procs[proc->ppid]->children.push_back(proc->pid);

    ++itr;
  }

  return;
}

Procs::~Procs()
{
  for ( auto proc: _procs ) {
    delete proc.second;
  }
}

const Proc& Procs::GetProc(pid_t pid) const
{
  auto itr = _procs.find(pid);
  if ( itr == _procs.end() )
    throw std::logic_error("getting non-existant pid");

  ProcPtr proc = itr->second;

  return *proc;
}

// eof
