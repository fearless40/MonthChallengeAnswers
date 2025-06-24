#include "averageai.hpp"
#include "RowCol.hpp"
#include "game.hpp"
#include <iostream>
#include <optional>
#include <random>
std::random_device rd("default");

void AverageAI::new_game(battleship::BoardDescription &game_input) {
  game = game_input;
  m_guess.resize(game_input.rows, game_input.cols);
  m_guess.fill(Status::unknown);
  m_next_guesses.clear();
  m_sunk.clear();
}

battleship::RowCol AverageAI::set_guess(battleship::RowCol g) {
  last_guess = g;
  ++m_nbr_guess;
  return last_guess;
}

std::optional<battleship::ShipPosition> AverageAI::guess() {
  // Check if we have any pre-generated guess we need to make and return

  if (m_next_guesses.size() > 0) {
    auto guess = m_next_guesses.back();
    m_next_guesses.pop_back();
    return set_guess(guess);
  }

  // Make a random guess
  auto rd_row =
      std::uniform_int_distribution<unsigned short>(0, game.rows.size - 1);
  auto rd_col =
      std::uniform_int_distribution<unsigned short>(0, game.cols.size - 1);

  // Pick a random point and then search for next guess.
  auto pos = battleship::RowCol{battleship::Row{rd_row(rd)},
                                battleship::Col{rd_col(rd)}};
  if (auto next_guess = m_guess.find_index_from(Status::unknown, pos);
      next_guess) {
    return set_guess(next_guess.value());
  } else {
    return {};
  }
}

void AverageAI::miss() {
  // Set the area as a miss
  m_guess.set(last_guess, Status::empty);
}

void AverageAI::hit() {
  // Calculate a grid of values to try and add them to the list
  m_guess.set(last_guess, Status::hit);

  auto up = last_guess - battleship::Row{1};
  auto down = last_guess + battleship::Row{1};
  auto left = last_guess - battleship::Col{1};
  auto right = last_guess + battleship::Col{1};

  if (m_guess.is_valid_index(up) && m_guess[up] == Status::unknown)
    m_next_guesses.push_back(up);

  if (m_guess.is_valid_index(down) && m_guess[down] == Status::unknown)
    m_next_guesses.push_back(down);

  if (m_guess.is_valid_index(left) && m_guess[left] == Status::unknown)
    m_next_guesses.push_back(left);
  if (m_guess.is_valid_index(right) && m_guess[right] == Status::unknown)
    m_next_guesses.push_back(right);
}

void AverageAI::sink(battleship::ShipID id) {
  m_sunk.push_back(id);
  m_guess.set(last_guess, Status::hit);
}
