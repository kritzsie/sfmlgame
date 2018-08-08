#include "util.hpp"

#include <physfs.h>

#include <sstream>
#include <string>
#include <vector>

namespace ke::util {
std::vector<char> readFile(std::string path) {
  PHYSFS_File* file = PHYSFS_openRead(path.c_str());
  std::size_t size = PHYSFS_fileLength(file);
  char* data = new char[size];
  if (file == nullptr) {
    std::stringstream ss;
    ss << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
    throw std::runtime_error(ss.str());
  }
  PHYSFS_readBytes(file, data, size);
  PHYSFS_close(file);
  return std::vector<char>(data, data + size);
}
}
