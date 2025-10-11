#include "verifymode.hpp"
#include "error.hpp"
#include "fileloader.hpp"
#include "iostream"
#include <algorithm>
#include <format>

int run_command_mode_verify(const commandline::ProgramOptions &opt) {
  ErrorReport errs;
  Game g = load_from_file(opt.filename, errs);

  print_game(g);

  if (g.report_game_is_valid(errs)) {
    std::cout << "Passed: No errors detected.\n";
  } else {
    std::cout << "Failed: Errors detected.\n";
  }

  if (errs) {
    std::ranges::for_each(errs.errors, [](const std::string &str) {
      std::cout << std::format("ERROR: {} \n", str);
    });
  }

  return 0;
}
