#include "bestai.hpp"
#include "Array2D.hpp"
#include "RowCol.hpp"
#include "game.hpp"
#include <algorithm>
#include <array>
#include <iterator>
#include <random>
using battleship::Col;
using battleship::Row;
using battleship::RowCol;

std::random_device rd2("default");

void BestAI::new_game(battleship::BoardDescription &game_input) {
  game = game_input;
  m_guess.resize(game_input.rows, game_input.cols);
  m_guess.fill(Status::unknown);
  m_prob.resize(game_input.rows, game_input.cols);
  m_prob.fill(0);
  m_sunk.clear();
}

battleship::RowCol BestAI::set_guess(battleship::RowCol g) {
  last_guess = g;
  ++m_nbr_guess;
  return last_guess;
}

void BestAI::clear_probability() noexcept { m_prob.fill(0); }

std::vector<int> BestAI::ships_still_alive() const {

  std::vector<int> sunk_ships;

  std::transform(m_sunk.begin(), m_sunk.end(), std::back_inserter(sunk_ships),
                 [](auto &value) { return value.size; });

  std::sort(sunk_ships.begin(), sunk_ships.end());

  std::vector<int> all_ships(game.max.size - game.min.size);
  std::vector<int> diff;

  std::iota(all_ships.begin(), all_ships.end(), game.min.size);

  std::set_difference(sunk_ships.begin(), sunk_ships.end(), all_ships.begin(),
                      all_ships.end(), std::back_inserter(diff));
  return diff;
}

int BestAI::test_cols_if_ship_fits(battleship::Row row,
                                   battleship::Col start_col,
                                   battleship::ShipID ship) {
  auto endCol = std::min(
      battleship::Col(start_col) + battleship::Col(ship.size), game.cols);

  for (auto c_hits = start_col; c_hits < endCol; ++c_hits) {
    if (m_guess[battleship::RowCol(row, c_hits)] != Status::unknown)
      return 0;
  }
  return 1;
}
int BestAI::test_rows_if_ship_fits(battleship::Col col,
                                   battleship::Row start_row,
                                   battleship::ShipID ship) {
  auto endRow = std::min(
      battleship::Row(start_row) + battleship::Row(ship.size), game.rows);

  for (auto r_hits = start_row; r_hits < endRow; ++r_hits) {
    if (m_guess[battleship::RowCol(r_hits, col)] != Status::unknown)
      return 0;
  }
  return 1;
}

void BestAI::generate_probability() noexcept {
  clear_probability();
  auto ships_left = ships_still_alive();

  for (auto row = battleship::Row(0); row < game.rows; ++row) {
    for (auto col = battleship::Col(0); col < game.cols; ++col) {

      for (auto ship_sz : ships_left) {
        m_prob[battleship::RowCol(row, col)] +=
            test_cols_if_ship_fits(row, col, battleship::ShipID(ship_sz));
        m_prob[battleship::RowCol(row, col)] +=
            test_rows_if_ship_fits(col, row, battleship::ShipID(ship_sz));
      }
      if (m_guess[battleship::RowCol(row, col)] == Status::hit) {
        std::array<RowCol, 4> positions{RowCol(row - battleship::Row(1), col),
                                        RowCol(row + battleship::Row(1), col),
                                        RowCol(row, col - battleship::Col(1)),
                                        RowCol(row, col + battleship::Col(1))};

        // Add extra weights for positions that have a ship that is hit.
        for (auto pos : positions) {
          if (m_prob.is_valid_index(pos)) {
            if (m_guess[pos] == Status::unknown)

              m_prob[pos] += game.max.size + 5;
          }
        }
      }
    }
  }
}

RowCol BestAI::get_highest_prob_of_ship() const noexcept {
  auto max_location = RowCol(Row(0), Col(0));
  int max_value = 0;
  for (auto r = Row(0); r < game.rows; ++r) {
    for (auto c = Col(0); c < game.cols; ++c) {
      if (max_value < m_prob[RowCol(r, c)]) {
        max_value = m_prob[RowCol(r, c)];
        max_location = RowCol(r, c);
      }
    }
  }
  return max_location;
}

std::optional<battleship::ShipPosition> BestAI::guess() {
  // Check if we have any pre-generated guess we need to make and return

  // if (m_next_guesses.size() > 0) {
  //   auto guess = m_next_guesses.back();
  //   m_next_guesses.pop_back();
  //   return set_guess(guess);
  // }
  //
  // Make a random guess
  auto rd_row =
      std::uniform_int_distribution<unsigned short>(0, game.rows.size - 1);
  auto rd_col =
      std::uniform_int_distribution<unsigned short>(0, game.cols.size - 1);

  // Pick a random point and then search for next guess.
  auto pos = battleship::RowCol{battleship::Row{rd_row(rd2)},
                                battleship::Col{rd_col(rd2)}};
  if (auto next_guess = m_guess.find_index_from(Status::unknown, pos);
      next_guess) {
    return set_guess(next_guess.value());
  } else {
    return {};
  }
}

void BestAI::miss() {
  // Set the area as a miss
  m_guess.set(last_guess, Status::empty);
}

void BestAI::hit() {
  // Calculate a grid of values to try and add them to the list
  m_guess.set(last_guess, Status::hit);

  auto up = last_guess - battleship::Row{1};
  auto down = last_guess + battleship::Row{1};
  auto left = last_guess - battleship::Col{1};
  auto right = last_guess + battleship::Col{1};

  // if (m_guess.is_valid_index(up) && m_guess[up] == Status::unknown)
  //   m_next_guesses.push_back(up);
  //
  // if (m_guess.is_valid_index(down) && m_guess[down] == Status::unknown)
  //   m_next_guesses.push_back(down);
  //
  // if (m_guess.is_valid_index(left) && m_guess[left] == Status::unknown)
  //   m_next_guesses.push_back(left);
  // if (m_guess.is_valid_index(right) && m_guess[right] == Status::unknown)
  //   m_next_guesses.push_back(right);
}

void BestAI::sink(battleship::ShipID id) {
  m_sunk.push_back(id);
  m_guess.set(last_guess, Status::hit);
}
