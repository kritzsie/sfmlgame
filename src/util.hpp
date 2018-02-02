#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <vector>

namespace util {
  std::vector<char> load_file(std::string);
}

namespace keng {
  using namespace util;
}

#endif // UTIL_HPP
