#pragma once
#include "error.hpp"
#include "game.hpp"
#include <filesystem>

Game load_from_file(std::filesystem::path filename, ErrorReport &error);