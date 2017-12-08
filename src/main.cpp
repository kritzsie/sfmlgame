#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <cstdlib>

#include "engine.hpp"

int main(int argc, const char* argv[]) {
  engine::Engine engine(engine::arglist(argv, argc + argv));

  return engine.run();
}
