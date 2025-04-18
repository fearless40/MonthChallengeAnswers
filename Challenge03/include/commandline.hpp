#pragma once
#include "programoptions.hpp"
#include <optional>

namespace commandline {
std::optional<ProgramOptions::Options> parse_command_line(int argc,
                                                          char *argv[]);
}
