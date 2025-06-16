#include "stupidai.hpp"
#include "RowCol.hpp"
#include "game.hpp"

std::optional<battleship::ShipPosition> StupidAI::guess() {
  return battleship::RowCol::random(battleship::Row(game.rows.size),
                                    battleship::Col(game.cols.size));
}
