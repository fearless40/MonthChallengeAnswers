#pragma once
#include "RowCol.hpp"
#include "collisions.hpp"
#include "gamelayout.hpp"
#include <cstddef>
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

  constexpr std::optional<std::size_t> ship_section_hit(RowCol const &pos) {
    int x = static_cast<int>(pos.col.size);
    int y = static_cast<int>(pos.row.size);

    if (!location.contains_point(x, y))
      return {};

    int yIndex = y - location.y;
    int xIndex = x - location.x;

    if (yIndex == 0)
      return static_cast<std::size_t>(xIndex);
    if (xIndex == 0)
      return static_cast<std::size_t>(yIndex);

    return {};
  }

  constexpr ShipDefinition id() const noexcept { return shiplength; }
};

using Ships = std::vector<Ship>;

enum class ShotStatus { hit, miss, invalid };

struct ShipCollision {
  ShotStatus status;
  ShipDefinition id;
  constexpr operator bool() const { return status == ShotStatus::hit; }
};

std::optional<Ships> random_ships(GameLayout const &layout);

constexpr ShipCollision shot_at(Ships const &ships, RowCol const &shot) {
  for (auto &ship : ships) {
    if (ship.location.contains_point((int)shot.col.size, (int)shot.row.size)) {
      return {ShotStatus::hit, ship.id()};
    }
  }
  return {ShotStatus::miss, 0};
}

constexpr bool ships_collide(Ship const &ship1, Ship const &ship2) {
  return aabb_collision(ship1.location, ship2.location);
}

std::optional<Ship> ship_at_position(Ships const &ships, RowCol pos);
// std::optional<Ship> ship_at_position(Ships const &ships, Row r, Col c) {
//   return ship_at_position(ships, RowCol{r, c});
// }
constexpr bool any_collisions(Ships const &ships) {
  return any_collision(ships, [](auto &ship) { return ship.location; });
}

// struct Ships {
//   std::vector<Ship> m_ships;
//
//   constexpr bool check_if_position_collides(AABB position) {
//     for (auto &ship : m_ships) {
//       if (aabb_collision(position, ship.location))
//         return true;
//     }
//     return false;
//   }
//
//   constexpr bool any_collisions() {
//     return any_collision(m_ships, [](auto &ship) { return ship.location;
//     });
//   }
//
//   std::optional<Ship> ship_at_position(RowCol pos) const {
//     return ship_at_position(pos.row, pos.col);
//   }
//   std::optional<Ship> ship_at_position(Row row, Col col) const;
//
//   bool generate_random_ships(GameLayout const &game);
// };

} // namespace battleship
