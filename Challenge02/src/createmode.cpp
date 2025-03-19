#include "filewriter.hpp"
#include "parser.hpp"
#include <algorithm>
#include <createmode.hpp>
#include <filewriter.hpp>
#include <format>
#include <game.hpp>
#include <iostream>

std::optional<Ship> parse_ship(std::string_view str) {
  namespace dsl = parser::dsl;
  std::string_view angle;
  RowCol position;
  std::uint16_t ship_id;

  auto parse_position = (dsl::letters_ignore_case +
                         dsl::int_parser<std::uint16_t>)([&position](auto str) {
    position = RowCol::from_string(str);
  });
  auto parse_ship = dsl::int_parser<std::uint16_t>(ship_id) >>
                    dsl::single_char<':'> >> parse_position >>
                    dsl::single_char<':'> >> dsl::letters_ignore_case(angle);
  if (parser::parse(str, parse_ship, parser::empty{})) {
    std::uint16_t w = 0;
    std::uint16_t h = 0;
    if (angle == "H" || angle == "h") {
      w = ship_id - 1;
    } else {
      h = ship_id - 1;
    }

    AABB pos{position.col, position.row, position.col + w, position.row + h};
    return Ship{ship_id, pos};
  }

  return {};
}

int run_command_mode_create(const commandline::ProgramOptions &opt) {
  ErrorReport report;
  Game game;
  game.rows = opt.rows;
  game.cols = opt.cols;
  game.minShipSize = opt.ship_smallest;
  game.maxShipSize = opt.ship_largest;

  size_t playerId = 0;
  for (auto &cPlayer : opt.players) {
    Player p;
    p.name = cPlayer.name;
    p.id = (uint32_t)playerId++;
    if (cPlayer.isRandom) {
      p.generate_random_ships(game, report);
    } else {

      for (auto placement : cPlayer.placements) {
        if (auto ship = parse_ship(placement); ship)
          p.ships.push_back(ship.value());
      }
    }
    game.players.push_back(p);
  }

  print_game(game);
  game.report_game_is_valid(report);
  print_player(game, game.players.front());

  if (filewriter::write_to_file(game, opt.filename,
                                filewriter::Format::Challenge02)) {
    report.errors.push_back("Could not write to file. ");
  }

  if (report) {
    std::ranges::for_each(report.errors, [](const std::string &str) {
      std::cout << std::format("ERROR: {} \n", str);
    });
  }

  return 0;
}
