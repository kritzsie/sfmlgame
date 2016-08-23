#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <cstdlib>

#include "engine.hpp"
#include "geometry.hpp"

int main(int argc, const char* argv[]) {
  auto engine = engine::Engine(engine::arglist(argv, argc + argv));

  return engine.exec();
}
