#pragma once
#include "RowCol.hpp"
#include "error.hpp"
#include <collisions.hpp>
#include <optional>
#include <string>
#include <vector>

enum class Orientation { Horizontal, Vertical };

const int UNSET = -1;

struct Game;
struct Player;

struct Ship {
  uint32_t shiplength;
  AABB location{UNSET, UNSET, UNSET, UNSET};
  constexpr Orientation orientation() const noexcept {
    if ((location.x2 - location.x) > (location.y2 - location.y))
      return Orientation::Horizontal;

    return Orientation::Vertical;
  }

  constexpr int row_size() const noexcept { return location.x2 - location.x; }

  constexpr int col_size() const noexcept { return location.y2 - location.y; }

  constexpr bool is_valid() const noexcept {
    return (row_size() == 0 && col_size() == shiplength - 1) ||
           (row_size() == shiplength - 1 && col_size() == 0);
  }

  constexpr uint32_t id() const noexcept { return shiplength; }
};

struct Player {
  uint32_t id;
  std::string name;
  std::vector<Ship> ships;

  constexpr bool check_if_position_collides(AABB position) {
    for (auto &ship : ships) {
      if (aabb_collision(position, ship.location))
        return true;
    }
    return false;
  }

  constexpr bool any_collisions() {
    return any_collision(ships, [](auto &ship) { return ship.location; });
  }

  std::optional<Ship> ship_at_position(std::uint16_t row,
                                       std::uint16_t col) const;

  bool generate_random_ships(Game const &game, ErrorReport &report);
};

struct Game {
  std::uint16_t rows;
  std::uint16_t cols;
  uint32_t minShipSize;
  uint32_t maxShipSize;
  std::vector<Player> players;

  constexpr static bool valid_row_col(std::uint16_t row, std::uint16_t col) {
    return row >= 0 && row < std::numeric_limits<std::uint16_t>::max() &&
           col >= 0 && col < std::numeric_limits<std::uint16_t>::max();
  }

  constexpr bool valid_pos(RowCol pos) const noexcept {
    return (pos.row >= 0 and pos.row <= rows) and
           (pos.col >= 0 and pos.col <= cols);
  }

  bool report_game_is_valid(ErrorReport &report) const noexcept;
};

void print_player(Game const &, Player const &);
void print_game(Game const &g);
