#include "guessallai.hpp"
#include "RowCol.hpp"
#include "game.hpp"
#include <optional>

std::optional<battleship::ShipPosition> GuessAllAi::guess() {

  if (m_col.size == game.cols.size) {
    ++m_row.size;
    m_col.size = 0;
  }
  if (m_row.size == game.rows.size)
    return {};

  auto col = m_col;
  ++m_col.size;
  return battleship::ShipPosition{m_row, col};
}
