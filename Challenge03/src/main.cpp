#include "baseconv.hpp"
#include "commandline.hpp"
#include "game.hpp"
#include "programoptions.hpp"
#include "stupidai.hpp"
#include <charconv>
#include <iostream>
#include <iterator>
#include <memory>

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

enum class InputAction { miss, hit, sink, endgame, quitprogram, unknownaction };

struct ParsedAction {
  InputAction action;
  std::size_t shipID;
};

ParsedAction parse_actions_from_input(std::string_view input) {
  if (input.empty())
    return {InputAction::unknownaction, 0};

  char c = input.front();

  switch (c) {
  case 'M':
  case 'm':
    return {InputAction::miss, 0};
  case 'H':
  case 'h':
    return {InputAction::hit, 0};
  case 'S':
  case 's': {
    std::size_t shipid = 0;
    auto res = std::from_chars(input.begin() + 1, input.end(), shipid);

    return {InputAction::sink, shipid};
  }
  case 'E':
  case 'e':
    return {InputAction::endgame, 0};
  case 'Q':
  case 'q':
    return {InputAction::quitprogram, 0};
  default:
    return {InputAction::unknownaction, 0};
  }
}

std::unique_ptr<AI> get_ai_by_id(std::size_t id) {
  switch (id) {
  default:
  case 0:
    return std::make_unique<StupidAI>();
  }
};

std::size_t get_ai_count() { return 1; }

int run_event_loop(ProgramOptions::Options opt) {
  // do nothing for now
  Battleship::BoardDescription game_desc{
      Battleship::Row{opt.rows_count}, Battleship::Col{opt.cols_count},
      Battleship::ShipSize{opt.ship_start}, Battleship::ShipSize{opt.ship_end}};

  std::cout << "Awaiting input: ";
  std::string input;
  auto ai = get_ai_by_id(opt.ai_id);
  ai->new_game(game_desc);

  while (true) {
    // Output the first guess from the AI
    auto guess = ai->guess();
    std::cout << guess.as_base26_fmt() << '\n';
    std::cin >> input;
    ParsedAction action = parse_actions_from_input(input);

    switch (action.action) {

    case InputAction::unknownaction:
      std::cout << "Unknown input. Quitting.\n";
      return 1;
    case InputAction::miss:
      ai->miss();
      break;
    case InputAction::hit:
      ai->hit();
      break;
    case InputAction::sink:
      ai->sink(Battleship::ShipID{action.shipID});
      break;
    case InputAction::endgame:
      ai->new_game(game_desc);
      break;
    case InputAction::quitprogram:
      std::cout << "Quitting hope you had fun.\n";
      return 0;
    }
  }
}

void display_ai_list(ProgramOptions::Options const &opt) {

  if (opt.print_ai_count_only) {
    std::cout << get_ai_count() << '\n';
    return;
  }

  for (std::size_t i = 0; i < get_ai_count(); ++i) {
    auto ai = get_ai_by_id(i);
    std::cout << "AI ID: " << i << '\n';
    std::cout << "Description: " << ai->description() << '\n';
  }
};

int main(int argc, char *argv[]) {
  auto opt_opt = commandline::parse_command_line(argc, argv);
  if (opt_opt) {
    auto &opt = opt_opt.value();
    switch (opt.mode) {
    case ProgramOptions::RunMode::run:
      run_event_loop(opt);
      break;
    case ProgramOptions::RunMode::ailist:
      display_ai_list(opt);
      break;
    case ProgramOptions::RunMode::help:
      return 0;
    case ProgramOptions::RunMode::version:
      std::cout << "Version 1.0\n";
      return 0;
    case ProgramOptions::RunMode::about:
      std::cout << "\nSee "
                   "https://github.com/fearless40/MonthChallenges/blob/main/"
                   "Challenge03/Challenge03.md for details. This program "
                   "follows the instructions of the challenge.\n";
      return 0;
    }
  }
}
