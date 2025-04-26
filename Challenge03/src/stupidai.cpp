#include "stupidai.hpp"
#include "RowCol.hpp"
#include "game.hpp"

Battleship::ShipPosition StupidAI::guess() {
  return RowCol::random(game.rows.size, game.cols.size);
}
