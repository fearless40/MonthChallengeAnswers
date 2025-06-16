#pragma once

#include "ai.hpp"
#include "game.hpp"

class StupidAI : public AI {
private:
  battleship::BoardDescription game;
  battleship::ShipPosition last_guess;

public:
  ~StupidAI() {}
  StupidAI() {};

  void new_game(battleship::BoardDescription &game_input) override {
    game = game_input;
  }

  void hit() override {

  };

  void miss() override {

  };

  void sink(battleship::ShipID shipId) override {

  };

  std::optional<battleship::ShipPosition> guess() override;

  const std::string_view description() const override {
    return "A very stupid guesser that does not check if it has guessed the "
           "same guess. Does not react to any game events. Will guess for "
           "eternity. ";
  }
};
