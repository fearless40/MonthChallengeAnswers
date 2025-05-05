#pragma once
#include "RowCol.hpp"
#include "collisions.hpp"
#include "gamelayout.hpp"
#include <optional>
#include <vector>

namespace battleship {

enum class Orientation { Horizontal, Vertical };
constexpr int UNSET = -1;
struct Ship {
  ShipDefinition shiplength;
  AABB location{UNSET, UNSET, UNSET, UNSET};
  constexpr Orientation orientation() const noexcept {
    if ((location.x2 - location.x) > (location.y2 - location.y))
      return Orientation::Horizontal;

    return Orientation::Vertical;
  }

  constexpr int row_size() const noexcept { return location.x2 - location.x; }

  constexpr int col_size() const noexcept { return location.y2 - location.y; }

  constexpr bool is_valid() const noexcept {
    return (row_size() == 0 && col_size() == shiplength.size - 1) ||
           (row_size() == shiplength.size - 1 && col_size() == 0);
  }

  constexpr ShipDefinition id() const noexcept { return shiplength; }
};

struct Ships {
  std::vector<Ship> m_ships;

  constexpr bool check_if_position_collides(AABB position) {
    for (auto &ship : m_ships) {
      if (aabb_collision(position, ship.location))
        return true;
    }
    return false;
  }

  constexpr bool any_collisions() {
    return any_collision(m_ships, [](auto &ship) { return ship.location; });
  }

  std::optional<Ship> ship_at_position(RowCol pos) const {
    return ship_at_position(pos.row, pos.col);
  }
  std::optional<Ship> ship_at_position(Row row, Col col) const;

  bool generate_random_ships(GameLayout const &game);
};

} // namespace battleship
