#include "commandline.hpp"
#include "programoptions.hpp"
#include <iostream>
#include <iterator>

void debug_program_options(ProgramOptions::Options &opt) {
  std::cout << "---------------" << '\n';
  std::cout << "Rows: " << opt.rows_count << '\n';
  std::cout << "Cols: " << opt.cols_count << '\n';
  std::cout << "AI ID:" << opt.ai_id << '\n';
  std::cout << "Ship Start Size: " << opt.ship_start << '\n';
  std::cout << "Ship End Size: " << opt.ship_end << '\n';
  std::cout << "Print AI Count: "
            << (opt.print_ai_count_only == true ? "true" : "false") << '\n';
}

int main(int argc, char *argv[]) {
  auto opt_opt = commandline::parse_command_line(argc, argv);
  if (opt_opt) {
    debug_program_options(opt_opt.value());
  }
}
