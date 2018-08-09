#pragma once

#include <physfs.h>

#include <SFML/System.hpp>

#include <cstddef>
#include <string>
#include <vector>

namespace ke::util {
std::vector<char> readFile(std::string);

class FileInputStream : public sf::FileInputStream {
private:
  PHYSFS_File* filehandle;

public:
  bool open(std::string);
  virtual sf::Int64 read(void*, sf::Int64);
  virtual sf::Int64 seek(sf::Int64);
  virtual sf::Int64 tell();
  virtual sf::Int64 getSize();

  FileInputStream();
  virtual ~FileInputStream();
};
}
