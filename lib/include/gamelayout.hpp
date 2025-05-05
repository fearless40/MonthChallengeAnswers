#pragma once

#include "RowCol.hpp"
#include <cstddef>
namespace battleship {
struct ShipDefinition {
  std::size_t size;

  auto operator<=>(ShipDefinition const &other) const = default;
  bool operator==(ShipDefinition const &other) const = default;
};

struct GameLayout {
  ShipDefinition minShipSize{2};
  ShipDefinition maxShipSize{5};
  Row nbrRows{10};
  Col nbrCols{10};

  constexpr bool is_valid() const {
    if (minShipSize.size <= 0 or maxShipSize.size >= nbrRows.size or
        maxShipSize.size >= nbrCols.size or minShipSize >= maxShipSize)
      return false;

    return true;
  }
};

} // namespace battleship
