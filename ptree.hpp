// $Header$
//
#ifndef PTREE_HPP
#define PTREE_HPP

#include <list>
#include <memory>
#include <string>

#include <sys/types.h>

extern const char* const EXE_NAME;

using std::string;

typedef std::list<string> str_list;
typedef std::list<pid_t> pid_list;

struct Proc {
  pid_t    pid;
  pid_t    ppid;
  string   comm;
  pid_list children;
  string   cmdline;
};
typedef Proc* ProcPtr;

#endif // PTREE
