#pragma once
#include "game.hpp"
#include <optional>

class AI {
public:
  virtual ~AI() {};
  virtual void new_game(Battleship::BoardDescription &game) = 0;
  virtual void miss() = 0;
  virtual void hit() = 0;
  virtual void sink(Battleship::ShipID shipId) = 0;

  // Return Empty guess to indicate no more guesses
  virtual std::optional<Battleship::ShipPosition> guess() = 0;
  virtual const std::string_view description() const = 0;
};
