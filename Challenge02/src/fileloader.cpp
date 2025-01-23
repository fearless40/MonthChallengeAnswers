#include "fileloader.hpp"
#include <format>
#include <fstream>
#include <iostream>
#include <string_view>

const size_t BUFFER_SIZE = 4096;

char skip_ws(std::ifstream &file)
{
    char c = file.get();
    while (c == ' ' || c == '\t' || c == '\v')
        c = file.get();

    return c;
}

std::vector<Ship> read_ship_data(std::ifstream &file, ErrorReport &error, int rowSize, int colSize)
{
    int rowCount = 0;
    int colCount = 0;
    std::vector<Ship> ships;

    while (file && rowCount < rowSize && colCount < colSize)
    {
        int mapValue;
        file >> mapValue;

        std::cout << "Read: " << mapValue << '\n';
        /*if (!file)
        {
            error("Unable to read value in file data.");
            return {};
        }*/

        if (mapValue > 0)
        {
            auto it = std::ranges::find(ships, static_cast<uint32_t>(mapValue), &Ship::shiplength);
            if (it != ships.end())
            {
                auto &loc = (*it).location;
                loc.x2 = colCount;
                loc.y2 = rowCount;
            }
            else
            {
                // Create a ship
                ships.push_back(Ship{(uint32_t)mapValue, {colCount, rowCount, UNSET, UNSET}});
            }
        }

        if (char c = file.get(); c == ',')
        {
            ++colCount;
        }
        else
        {
            c = skip_ws(file);
            if (c == ',')
                ++colCount;
        }

        /*if (file.peek() != ',')
        {
            char c;
            do
            {
                c = (char)file.get();
            } while (c == ' ' || c == '\t' || c == '\v');
            file.seekg(-1, std::ios_base::cur);
        }

        if (file.peek() == ',')
        {
            file.seekg(1, std::ios_base::cur);
        }*/

        if (file.peek() == '\n' || file.peek() == '\r')
        {
            if (colCount != colSize - 1 || colCount == 0)
            {
                error(std::format("Found only this many cols: {} out of {}", colCount + 1, colSize));
                return {};
            }

            ++rowCount;
            colCount = 0;
            file >> std::ws;
        }
    }

    if (rowCount != rowSize)
    {
        error(std::format("Found only this many rows: {} out of {}", rowCount + 1, rowSize));
        return {};
    }

    return ships;
}

Game load_from_file(std::filesystem::path filename, ErrorReport &error)
{
    if (!std::filesystem::exists(filename))
    {
        error(std::format("File not found: {}", "hmm"));
        return {};
    }

    std::ifstream file{filename};
    // char buff[BUFFER_SIZE];

    unsigned int rowCount = 0;
    unsigned int colCount = 0;
    size_t nbrPlayers;

    file >> rowCount >> colCount >> nbrPlayers;
    if (!file)
    {
        error("Unable to read rowcount, colcount or nbrplayers");
        return {};
    }

    Game ret{(uint16_t)rowCount, (uint16_t)colCount, 999999, 0};
    ret.players.reserve(nbrPlayers);

    for (int playerId = 0; playerId < nbrPlayers; ++playerId)
    {
        std::string playerName;
        file >> playerName;
        if (!file)
        {
            error("Unable to read player name");
            return {};
        }
        std::cout << "Reading player: " << playerName << '\n';

        ret.players.emplace_back(playerId, playerName);
        auto &player = ret.players.back();

        player.ships = read_ship_data(file, error, ret.rows, ret.cols);
    }

    return ret;
    /*while (!file.eof())
    {
        file.read(buff, BUFFER_SIZE);
        auto charsRead = file.gcount();
        const std::string_view file_data(buff, charsRead < BUFFER_SIZE ? charsRead : BUFFER_SIZE);


    }*/
}
