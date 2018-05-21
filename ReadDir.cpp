// This file implements a simple class to read a directory.
// 
#include "ReadDir.hpp"

#include <stdexcept>

#include <errno.h>
#include <string.h>

ReadDir::ReadDir(const char* path)
  : _dh(nullptr)
{
  if (path == nullptr) {
    throw std::invalid_argument("null directory path");
  }

  _dh = opendir(path);
  if (_dh == nullptr) {
    throw std::runtime_error(strerror(errno));
  }
}

ReadDir::~ReadDir()
{
  if (_dh) {
    closedir(_dh);
  }
}

dirent* ReadDir::ReadNext()
{
  errno = 0;
  dirent* de = readdir(_dh);
  if (de == nullptr && errno != 0) {
    throw std::runtime_error(strerror(errno));
  }

  return de;
}

// eof
