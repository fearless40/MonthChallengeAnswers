#pragma once

#include "RowCol.hpp"
#include "ai.hpp"
#include "game.hpp"

class GuessAllAi : public AI {
private:
  Battleship::BoardDescription game;
  Battleship::ShipPosition last_guess;
  battleship::Row m_row{0};
  battleship::Col m_col{0};

public:
  ~GuessAllAi() {}

  void new_game(Battleship::BoardDescription &game_input) override {
    game = game_input;
  }

  void hit() override {

  };

  void miss() override {

  };

  void sink(Battleship::ShipID shipId) override {

  };

  std::optional<Battleship::ShipPosition> guess() override;

  const std::string_view description() const override {
    return "Guess through the entire game starting at Row 0 Col 0. Very "
           "silly. ";
  }
};
