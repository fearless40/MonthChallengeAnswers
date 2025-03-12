#include "commandline.hpp"

int main(int argc, char const *argv[]) {
  auto opt = commandline::parse(argc, argv);
  if (!opt)
    return -1;

  auto optv = opt.value();

  /*
      auto board = load_from_file(filename);
      if (!board)
      {
          error("File could not be loaded. Quitting.");
          return -1;
      }

      if (runAutomated)
      {
          non_interactive_mode(board.value(), guesses);
          std::flush(std::cout);
          return 0;
      }
      else
      {
          interactive_mode(board.value(), filename);
          return 0;
      }

      */
}
