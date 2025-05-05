#include "ship.hpp"
#include "RandomUtil.hpp"
#include "collisions.hpp"
#include <terminal.hpp>

namespace battleship {

std::optional<Ship> Ships::ship_at_position(Row row, Col col) const {
  for (auto &ship : m_ships) {
    int y = (int)row.size;
    int x = (int)col.size;
    if (ship.location.contains_point(x, y))
      return ship;
  }
  return {};
}

bool Ships::generate_random_ships(GameLayout const &game) {

  // Validate that it is a valid game board
  if (!game.is_valid())
    return false;

  m_ships.clear();
  m_ships.reserve(game.maxShipSize.size - game.minShipSize.size + 1);

  for (ShipDefinition ship_id = game.minShipSize;
       ship_id.size < game.maxShipSize.size + 1; ++ship_id.size) {

    auto const size_vector = m_ships.size();
    for (size_t count_attempts = 0; count_attempts < 1000; ++count_attempts) {
      Orientation layout = randomns::coin_flip() == true
                               ? Orientation::Horizontal
                               : Orientation::Vertical;
      std::uint16_t width =
          (layout == Orientation::Vertical ? 0 : ship_id.size - 1);
      std::uint16_t height =
          (layout == Orientation::Horizontal ? 0 : ship_id.size - 1);

      int row = randomns::between(0, game.nbrRows.size - 1 - height);
      int col = randomns::between(0, game.nbrCols.size - 1 - width);
      AABB position{col, row, col + width, row + height};
      if (!check_if_position_collides(position)) {
        m_ships.emplace_back(ship_id, position);
        // report(std::format("Player {} generated: {} after {} tries", name,
        // ship_id, count_attempts));
        break;
      } else {
        ++count_attempts;
      }
    }
    if (m_ships.size() == size_vector) {
      return false;
    }
  }
  return true;
}

} // namespace battleship
