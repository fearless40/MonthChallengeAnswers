#pragma once
#include <cstddef>

namespace ProgramOptions {

enum class RunMode { run, ailist, version, help, about };

struct Options {
  RunMode mode;
  std::size_t rows_count{10};
  std::size_t cols_count{10};
  std::size_t ai_id{0};
  std::size_t ship_start{2};
  std::size_t ship_end{5};
  bool print_ai_count_only{true};
};
} // namespace ProgramOptions
