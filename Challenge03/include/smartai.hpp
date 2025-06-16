#pragma once

#include "RowCol.hpp"
#include "ai.hpp"
#include "game.hpp"

class GuessAllAi : public AI {
private:
  battleship::BoardDescription game;
  battleship::ShipPosition last_guess;
  battleship::Row m_row{0};
  battleship::Col m_col{0};

public:
  ~GuessAllAi() {}

  void new_game(battleship::BoardDescription &game_input) override {
    game = game_input;
    m_row = battleship::Row(0);
    m_col = battleship::Col(0);
  }

  void hit() override {

  };

  void miss() override {

  };

  void sink(battleship::ShipID shipId) override {

  };

  std::optional<battleship::ShipPosition> guess() override;

  const std::string_view description() const override {
    return "Guess through the entire game starting at Row 0 Col 0. Very "
           "silly. ";
  }
};
