#pragma once

#include "Array2D.hpp"
#include "RowCol.hpp"
#include "ai.hpp"
#include "game.hpp"

class AverageAI : public AI {
private:
  enum class Status { unknown, empty, hit };
  battleship::BoardDescription game;
  battleship::ShipPosition last_guess;
  battleship::Array2D<Status> m_guess;
  std::vector<battleship::RowCol> m_next_guesses;
  std::size_t m_nbr_guess{0};
  std::vector<battleship::ShipID> m_sunk;

private: // Methods
  battleship::RowCol set_guess(battleship::RowCol g);

public:
  ~AverageAI() {}

  void new_game(battleship::BoardDescription &game_input) override;
  void hit() override;
  void miss() override;
  void sink(battleship::ShipID shipId) override;
  std::optional<battleship::ShipPosition> guess() override;

  const std::string_view description() const override {
    return "Guess randomly, when hitting a ship guess around the ship.";
  }
};
