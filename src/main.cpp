#include "engine.hpp"

int main(int argc, const char* argv[]) {
  keng::Engine engine(keng::arglist(argv, argc + argv));

  return engine.exec();
}
