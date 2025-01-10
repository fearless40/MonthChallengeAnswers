#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <optional>

struct CommandLineOptions {
    bool interactive {false};
    bool verbose {false};
    bool nomem {false};
    std::string fileToParse {};
    std::vector<std::string> queries;

};

std::optional<CommandLineOptions> parse(int argc, char const * argv[]);