#include "util.hpp"

#include <physfs.h>

#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace ke::util {
std::vector<char> readFile(std::string path) {
  if (PHYSFS_isInit() == 0) {
    throw std::runtime_error(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
  }

  PHYSFS_File* file = PHYSFS_openRead(path.c_str());
  if (file == nullptr) {
    throw std::runtime_error(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
  }

  std::size_t size = PHYSFS_fileLength(file);
  if (size == 0) {
    return std::vector<char>();
  }

  char* data = new char[size];
  PHYSFS_readBytes(file, data, size);
  PHYSFS_close(file);
  return std::vector<char>(data, data + size);
}
}
