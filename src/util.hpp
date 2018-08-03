#pragma once

#include <string>
#include <vector>

namespace util {
  std::vector<char> load_file(std::string);
}

namespace keng {
  using namespace util;
}
