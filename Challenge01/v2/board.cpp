#include "board.hpp"





GameBoard GameBoard::load_from_file(std::filesystem::path file)
{
    if (!std::filesystem::exists(file))
    {
        //error("Filename not found.");
        //std::cout << "Filename: " << filename << '\n';
    }

    
    
    std::ifstream file{filename};
    char buff[255];

    unsigned int rowCount = 0;
    unsigned int colCount = 0;

    {
        file.getline(buff, 256);
        auto rowParse = parse_number_positive(buff);
        if (std::holds_alternative<const std::string_view>(rowParse))
        {
            error(std::get<const std::string_view>(rowParse));
        }
        else
        {
            rowCount = std::get<int>(rowParse);
        }
    }

    {
        file.getline(buff, 256);
        auto colParse = parse_number_positive(buff);
        if (std::holds_alternative<const std::string_view>(colParse))
        {
            error(std::get<const std::string_view>(colParse));
        }
        else
        {
            colCount = std::get<int>(colParse);
        }
    }

    if (rowCount == 0 || rowCount > 65535)
    {
        error("Rowcount too large or 0");
        return {};
    }

    if (colCount == 0 || colCount > 65535)
    {
        error("Colcount too large or 0");
        return {};
    }

    std::cout << std::format("Row Count: {}\nCol Count: {}\n", rowCount, colCount);

    std::vector<std::int16_t> data;
    std::vector<std::string> test;
    auto stringView = std::views::istream<std::string>(file);

    std::size_t nbrRows = 0;
    for (auto &l : stringView)
    {
        ++nbrRows;
        std::stringstream ss{l};
        std::string temp;
        std::size_t nbrCols = 0;
        while (std::getline(ss, temp, ','))
        {
            auto ret = parse_number(temp);
            if (std::holds_alternative<int>(ret))
            {
                auto val = std::get<int>(ret);
                if (val < -32768 || val > 32767)
                {
                    error(std::format("Data value to large at: row:{} col:{}", nbrRows, nbrCols));
                    return {};
                }
                data.push_back(static_cast<std::int16_t>(val));
                ++nbrCols;
            }
            else
            {
                error(std::get<const std::string_view>(ret));
                std::cout << std::format("Found on row: {}, col {}", nbrRows, nbrCols) << '\n';
                break;
            }
        }

        if (nbrCols != colCount)
        {
            error(std::format("Invalid number of cols on row: {}", nbrRows));
            return {};
        }
    }
    if (nbrRows != rowCount)
    {
        error("Not enough rows in the data.");
        return {};
    }

    return {{rowCount, colCount, std::move(data)}};
}
}