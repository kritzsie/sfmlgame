#include "util.hpp"

#include <fstream>
#include <ios>
#include <string>
#include <vector>

namespace util {
  std::vector<char> load_file(std::string name) {
    std::ifstream file(name, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    std::vector<char> buffer(size);

    file.seekg(0, std::ios::beg);

    file.read(buffer.data(), size);
    return buffer;
  }
}
