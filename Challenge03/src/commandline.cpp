#include "commandline.hpp"
#include "clipp.h"
#include "programoptions.hpp"
#include <iostream>
namespace commandline {
std::optional<ProgramOptions::Options> parse_command_line(int argc,
                                                          char *argv[]) {
  ProgramOptions::Options opt{};
  bool print_ai_count;
  bool print_help{false};
  bool print_version{false};
  bool print_about{false};

  using namespace clipp;

  auto cli = (clipp::option("--rows") & value("Number of rows", opt.rows_count),
              option("--cols") & value("Number of columns", opt.cols_count),
              option("--ai") & clipp::value("AI ID selector", opt.ai_id),
              (option("--ships") & value("Smallest ship size", opt.ship_start) &
               value("Largest ship size", opt.ship_end))) |
             (option("--aicount").set(opt.print_ai_count_only, true)) |
             (option("help").set(print_help, true)) |
             (option("version").set(print_version, true)) |
             (option("about").set(print_about, true));

  if (clipp::parse(argc, argv, cli)) {
    if (print_help) {
      std ::cout << clipp::make_man_page(cli, "challenge03");
      return {};
    } else if (print_version) {
      std ::cout << "Version: 1.0\n";
      return {};
    } else if (print_about) {
      std::cout << "\nSee "
                   "https://github.com/fearless40/MonthChallenges/blob/main/"
                   "Challenge03/Challenge03.md for details. This program "
                   "follows the instructions of the challenge.\n";
      return {};
    }

    return opt;
  } else {
    std::cout << clipp::usage_lines(cli, "challenge03");
    return {};
  }
}
} // namespace commandline
