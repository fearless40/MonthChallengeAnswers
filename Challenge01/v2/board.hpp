#pragma once

#include <filesystem>

class GameBoard
{
    bool mIsValid{false};
    std::uint16_t mRows;
    std::uint16_t mCols;
    std::vector<std::int16_t> data;

  public:
    operator bool()
    {
        return mIsValid;
    }

    static GameBoard load_from_file(std::filesystem::path file);
};