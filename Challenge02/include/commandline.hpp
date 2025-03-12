#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace commandline {

enum class RunMode { Verify, Create, Interactive, Help, Quit };

struct Players {
  std::string name;
  std::vector<std::string> placements;
  bool isRandom{false};
};

struct ProgramOptions {
  RunMode mode{RunMode::Quit};
  std::string filename;
  std::uint16_t rows{10};
  std::uint16_t cols{10};
  std::uint16_t ship_smallest{2};
  std::uint16_t ship_largest{5};
  std::vector<Players> players;
};

ProgramOptions parse(int argc, char *argv[]);
const ProgramOptions &get_program_options();
} // namespace commandline
