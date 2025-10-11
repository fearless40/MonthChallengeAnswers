#pragma once

#include "Array2D.hpp"
#include "RowCol.hpp"
#include "ai.hpp"
#include "game.hpp"

class BestAI : public AI {
private:
  enum class Status { unknown, empty, hit };
  battleship::BoardDescription game;
  battleship::ShipPosition last_guess;
  battleship::Array2D<Status> m_guess;
  battleship::Array2D<int> m_prob; // Probality of spot having a ship
  std::size_t m_nbr_guess{0};
  std::vector<battleship::ShipID> m_sunk;

private: // Methods
  battleship::RowCol set_guess(battleship::RowCol g);
  std::vector<int> ships_still_alive() const;
  void clear_probability() noexcept;
  battleship::RowCol get_highest_prob_of_ship() const noexcept;
  void generate_probability() noexcept;

  int test_cols_if_ship_fits(battleship::Row row, battleship::Col start_col,
                             battleship::ShipID ship);
  int test_rows_if_ship_fits(battleship::Col col, battleship::Row start_row,
                             battleship::ShipID ship);

public:
  ~BestAI() {}

  void new_game(battleship::BoardDescription &game_input) override;
  void hit() override;
  void miss() override;
  void sink(battleship::ShipID shipId) override;
  std::optional<battleship::ShipPosition> guess() override;

  const std::string_view description() const override {
    return "Guess using statistics, based on best algo.";
  }
};
