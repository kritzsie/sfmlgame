#include "engine.hpp"
#include "types.hpp"

int main(int argc, char* argv[]) {
  ke::Engine engine(ke::StringList(argv, argc + argv));

  return engine.main();
}
