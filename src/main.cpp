#include "engine.hpp"

int main(int argc, char* argv[]) {
  ke::Engine engine(ke::ArgList(argv, argc + argv));

  return engine.main();
}
