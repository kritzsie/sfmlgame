#include "engine.hpp"

int main(int argc, char* argv[]) {
  ke::Engine engine(ke::arglist(argv, argc + argv));

  return engine.exec();
}
