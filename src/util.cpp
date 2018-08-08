#include "util.hpp"

#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <vector>

namespace ke::util {
  std::vector<char> load_file(std::string name) {
    std::ifstream file(name, std::ios::binary | std::ios::ate);
    if (file.fail()) {
      std::cerr << "Failed to load file \"" + name + "\".\n";
      return std::vector<char>(0);
    }
    std::streamsize size = file.tellg();
    std::vector<char> buffer(size);

    file.seekg(0, std::ios::beg);

    file.read(buffer.data(), size);
    return buffer;
  }
}
