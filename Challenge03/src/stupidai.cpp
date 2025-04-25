#include "stupidai.hpp"
#include "game.hpp"

Battleship::ShipPosition StupidAI::guess() {
  return {Battleship::Row{1}, Battleship::Col{1}};
}
