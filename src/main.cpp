#include "engine.hpp"

using engine::arglist;
using engine::Engine;

int main(int argc, const char* argv[]) {
  Engine engine(arglist(argv, argc + argv));

  return engine.exec();
}
