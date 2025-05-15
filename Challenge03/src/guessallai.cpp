#include "guessallai.hpp"
#include "RowCol.hpp"
#include "game.hpp"
#include <concepts>
#include <optional>

std::optional<Battleship::ShipPosition> GuessAllAi::guess() {

  if (m_col.size == game.cols.size)
    ++m_row.size;
  if (m_row.size == game.rows.size)
    return {};

  auto col = m_col;
  ++m_col.size;
  return Battleship::ShipPosition{m_row, col};
}
