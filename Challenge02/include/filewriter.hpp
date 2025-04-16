#pragma once
#include "error.hpp"
#include "game.hpp"

namespace filewriter {

enum class FileVersion { Challenge2, Challenge3 };

ErrorReport &write_file(ErrorReport &report, Game &game, std::string path,
                        FileVersion version = FileVersion::Challenge2);

} // namespace filewriter
