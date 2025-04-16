#include "filewriter.hpp"
#include "error.hpp"
#include <fstream>

namespace filewriter {

ErrorReport &write_player_challenge2(ErrorReport &error, std::fstream &file,
                                     const Game &game, const Player &player) {
  file << player.name << '\n';
  for (int row = 0; row < game.rows; ++row) {
    for (int col = 0; col < game.cols; ++col) {
      auto opt = player.ship_at_position(row, col);
      if (opt) {
        file << opt.value().id();
      } else {
        file << 0;
      }

      if (col < game.cols - 1)
        file << ',';
    }

    file << '\n';
  }
  return error;
}

ErrorReport &write_file(ErrorReport &report, Game &game, std::string path,
                        FileVersion version) {

  std::fstream file(path);

  if (file.bad()) {
    report("Could not write to file for some reason.");
    return report;
  }

  // Insert "Header"
  file << game.rows << '\n';
  file << game.cols << '\n';
  file << game.players.size() << '\n';
  for (auto &player : game.players) {
    write_player_challenge2(report, file, game, player);
    file << '\n';
  }

  file.close();
  return report;
}
} // namespace filewriter
