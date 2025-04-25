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

  auto run_cli =
      (command("run").set(opt.mode, ProgramOptions::RunMode::run),
       (option("--rows") & value("Number of rows", opt.rows_count),
        option("--cols") & value("Number of columns", opt.cols_count),
        option("--ai") & clipp::value("AI ID selector", opt.ai_id),
        (option("--ships") & value("Smallest ship size", opt.ship_start) &
         value("Largest ship size", opt.ship_end))));
  auto ai_cli = (command("ai").set(opt.mode, ProgramOptions::RunMode::ailist),
                 option("--details").set(opt.print_ai_count_only, true));

  auto help_cli =
      (command("help").set(opt.mode, ProgramOptions::RunMode::help));
  auto version_cli =
      (command("version").set(opt.mode, ProgramOptions::RunMode::version));
  auto about_cli =
      (command("about").set(opt.mode, ProgramOptions::RunMode::about));

  auto cli = run_cli | ai_cli | help_cli | version_cli | about_cli;

  if (clipp::parse(argc, argv, cli)) {
    if (opt.mode == ProgramOptions::RunMode::help) {
      std ::cout << clipp::make_man_page(cli, "challenge03");
      return opt;
    }

  } else {
    std::cout << clipp::usage_lines(cli, "challenge03");
    return {};
  }
  return opt;
}
} // namespace commandline
