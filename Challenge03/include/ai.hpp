#pragma once
#include "game.hpp"

class AI {
public:
  virtual ~AI() {};
  virtual void new_game(Battleship::BoardDescription &game) = 0;
  virtual void miss() = 0;
  virtual void hit() = 0;
  virtual void sink(Battleship::ShipID shipId) = 0;
  virtual Battleship::ShipPosition guess() = 0;
  virtual const std::string_view description() const = 0;
};
