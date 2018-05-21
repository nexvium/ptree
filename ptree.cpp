// ptree - print a process tree
// vim: ts=8 sts=2 sw=2 et
//
// Hack to try to duplicate Solaris' ptree functionality on Linux.  Linux's
// pstree displays either every process or just the descendants of the given
// pid.  This program displays just the given pid's ancestors and descendants.
// It also outputs a more intuitive format, IMO.

#include "ptree.hpp"
#include "Procs.hpp"

#include <iostream>
#include <cassert>
#include <vector>

#include <unistd.h>

const char* const EXE_NAME = "ptree";
static const char VERSION[] = "v1.0.0";

const pid_t INIT_PROC_PID    = 1; // init or systemd
const pid_t KTHREAD_PROC_PID = 2; // kernel thread daemon

void Usage(void)
{
  std::cout << "usage: " << EXE_NAME << " [opt] [pid1 pid2 ...]\n"
            << "\n"
            << "options:\n"
            << "\n"
            << "  -h  output this usage message\n"
            << "  -k  output kernel threads too\n"
            << "  -v  output version number" << std::endl;
}

void OutputProcess(const Proc& proc, int level)
{
  for (int i = 0; i < level; ++i)
    std::cout << "  ";
  std::cout << proc.pid << " " << proc.cmdline << std::endl;
}

// Output the processes that are ancestors of pid.
// Return the number of processes output.
int OutputAncestors(Procs& procs, pid_t pid)
{
  std::vector<pid_t> pids;
  while (pid != INIT_PROC_PID && pid != KTHREAD_PROC_PID) {
    pid = procs.GetProc(pid).ppid;
    pids.push_back(pid);
  }

  int level = 0;
  for (int i = pids.size(); i > 0; i--) {
    Proc proc = procs.GetProc(pids[i - 1]);
    OutputProcess(proc, level++);
  }

  return level;
}

// Output the processed identified by pid as well as all its descendants.
void OutputTree(Procs& procs, pid_t pid, int level = 0)
{
  Proc proc = procs.GetProc(pid);
  OutputProcess(proc, level);
  for (auto child: proc.children)
    OutputTree(procs, child, level + 1);
}

int main(int argc, char* argv[])
{
  pid_list pids;
  Procs    procs;
  bool     output_kthreads = false;

  opterr = 0;
  char opt;
  while ((opt = getopt(argc, argv, "hkv")) != -1) {
    switch (opt) {
      case 'h':
        Usage();
        exit(EXIT_SUCCESS);
        break;
      case 'v':
        std::cout << VERSION << std::endl;
        exit(EXIT_SUCCESS);
        break;
      case 'k':
        output_kthreads = true;
        break;
      default: /* '?' */
        std::cerr << EXE_NAME << ": invalid option -- -" << (char)optopt
                  << std::endl;
        exit(EXIT_FAILURE);
    }
  }

  for (auto i = optind; i < argc; i++) {
    int pid = atoi(argv[i]);
    if (pid < INIT_PROC_PID) {
      std::cerr << EXE_NAME << ": invalid pid -- " << argv[i] << std::endl;
      exit(EXIT_FAILURE);
    }
    pids.push_back(pid);
  }

  if ( pids.empty() ) {
    // If no pids are specified, output a tree of all the processes (and all
    // the kernel threads if requested).
    OutputTree(procs, INIT_PROC_PID);
    if ( output_kthreads ) {
      OutputTree(procs, KTHREAD_PROC_PID);
    }
  }
  else {
    // Otherwise output the ancestors followed by the descendants of each pid.
    for ( auto pid: pids ) {
      if ( procs.HasPid(pid) ) {
        int level = OutputAncestors(procs, pid);
        OutputTree(procs, pid, level);
      }
      else {
        std::cerr << EXE_NAME << ": no such such pid -- " << pid << std::endl;
      }
    }
  }

  return EXIT_SUCCESS;
}

// eof
