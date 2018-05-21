// This file declares a simple class to read a directory.
//
#ifndef _READDIR_HPP_
#define _READDIR_HPP_

#include <dirent.h>

class ReadDir
{
  public:
    explicit ReadDir(const char* path);
    explicit ReadDir(const ReadDir&) = delete; // disallow copy

    ~ReadDir();

    dirent* ReadNext();

    void operator=(const ReadDir&) = delete; // disallow assignment

  private:
    DIR* _dh;
};

#endif
