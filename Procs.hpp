// This file declares a class that encapsulates a list of all the processes
// that are running when the class is instantiated.
#ifndef _PROCS_HPP_
#define _PROCS_HPP_

#include "ptree.hpp"

#include <map>

typedef std::map<pid_t, ProcPtr> pid_map;
typedef std::pair<pid_t, ProcPtr> pid_pair;

class Procs
{
  public:
    Procs(void);
    Procs(const Procs&) = delete;   // disallow copying

    ~Procs();

    // throws exception if pid does not exist
    const Proc& GetProc(const pid_t pid) const;

    bool HasPid(const pid_t pid) const
    {
      return _procs.find(pid) != _procs.end();
    }

    void operator=(const Procs&) = delete; // disallow assignment

  private:
    pid_map _procs;
};

#endif
