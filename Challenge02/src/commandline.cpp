#include "commandline.hpp"
#include "clipp.h"
#include <iostream>

namespace commandline {
ProgramOptions parse(int argc, char *argv[]) {
  using namespace clipp;

  ProgramOptions opt;

  auto verifyCli = (clipp::command("verify").set(opt.mode, RunMode::Verify),
                    value("File to load", opt.filename) %
                        "A test file generated by the generate command.");

  auto createCli =
      (clipp::command("create").set(opt.mode, RunMode::Create) &
           value("file to output", opt.filename) %
               "Output file after creating the game.",
       (option("--row") &
        value("Numer of rows", opt.rows) % "Number of rows in the game."),
       (option("--col") & value("Number of columns", opt.cols) %
                              "Number of columns in the game."),
       (option("--ships") &
        value("Smallest ship", opt.ship_smallest) %
            "Smallest ship in the game" &
        value("Largest ship", opt.ship_largest) % "Largest ship in the game"),
       (repeatable(
           (option("--player") &
            value("player name").call([&](const char *input) {
              opt.players.push_back({input});
            })),
           option("--random").call([&]() {
             opt.players.back().isRandom = true;
           }) | (option("--place") &
                 repeatable(value(match::prefix_not("--"),
                                  "Position string: shipId:ColRow:Orientation")
                                .call([&](const std::string &text) {
                                  opt.players.back().placements.push_back(text);
                                }))))));

  auto displayCli = (clipp::command("display"));

  auto cli = (verifyCli | createCli | displayCli |
                  command("help").set(opt.mode, RunMode::Quit),
              option("-v", "--version")
                  .call([] { std::cout << "version 1.0\n\n"; })
                  .doc("show version"));

  auto result = clipp::parse(argc, argv, cli);

  if (!result) {
    std::cout << "Usage: \n" << clipp::usage_lines(cli, "challenge02") << '/n';
    return {};
  }

  if (opt.mode == RunMode::Help) {
    std::cout << clipp::make_man_page(cli, "challenge02") << '/n';
    return {};
  }

  // Verify the options are correct before returning
  if (opt.ship_smallest < 0 || opt.ship_largest > opt.rows ||
      opt.ship_largest > opt.cols) {
    std::cout
        << "ERROR: Invalid defaults. Ships too small or ships too large.\n";
    exit(-1);
  }

  return opt;
  // Validate the files before continuing.

  /*
      switch (opt.mode)
      {
      case RunMode::Run: {
          auto fileProgram = ensure_file_exists(testProgram);
          if (!fileProgram)
          {
              std::cout << "The program to test was not found: " << testProgram
  << '/n'; return {};
          }

          opt.testProgram = fileProgram.value();

          auto fileTest = ensure_file_exists(testFile);
          if (!fileTest)
          {
              std::cout << "The test file is not found: " << testFile << '/n';
              return {};
          }

          opt.testFile = fileTest.value();
      }
      break;

      case RunMode::Generate:

          if (does_file_exists(testFile))
          {
              std::cout << "\nFound an existing test file at: " << testFile <<
  '\n'; std::cout << "Generate a new file (y/n):"; std::string overwrite{};
              std::cin >> overwrite;
              if (overwrite.find('n') != std::string::npos)
              {
                  std::cout << "\nWill not generate new test file and test data.
  Quitting.\n"; return {};
              }

              opt.overwrite = true;
          }
          opt.testFile = testFile;
          break;

      case RunMode::Help:
          return {};

      case RunMode::Quit:
          return {};

      case RunMode::Interactive:
      default:
          break;
      }

      GlobalOptions = opt;
      return opt;
  }
  */
}

void print_options(const ProgramOptions &opt) {
  std::cout << "Printing program options...\n";
  std::cout << "Read in: \n";
  std::cout << "Mode: " << (int)opt.mode << '\n';
  std::cout << "Filename: " << opt.filename << '\n';
  std::cout << "Rows: " << opt.rows << '\n';
  std::cout << "Cols: " << opt.cols << '\n';
  std::cout << "Smallest Ship: " << opt.ship_smallest << '\n';
  std::cout << "Largest Ship: " << opt.ship_largest << '\n';
  std::cout << "Numer of players: " << opt.players.size() << '\n';

  for (auto &player : opt.players) {
    std::cout << "\tPlayer Name: " << player.name << '\n';
    std::cout << "\tIs random: " << player.isRandom << '\n' << '\t';
    for (auto &placements : player.placements) {
      std::cout << placements << " ";
    }
  }
  std::cout << '\n';
}

} // namespace commandline
