#include <iostream>
#include <iterator>

int main(int argc, char *argv[]) {
  std::cout << "Hello from challenge03: " << '\n';
  std::cout << "Argc = " << argc << '\n';
  for (std::size_t arg = 0; arg < argc; ++arg) {
    std::cout << "Arg" << arg << " = " << argv[arg] << '\n';
  }
  return 0;
}
