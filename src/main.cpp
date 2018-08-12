#include "engine.hpp"

int main(int argc, char* argv[]) {
  ke::Engine engine(ke::arglist_t(argv, argc + argv));

  return engine.main();
}
