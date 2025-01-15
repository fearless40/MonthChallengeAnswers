#include "clipp.h"
#include <algorithm>
#include <charconv>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>
#include <variant>
#include <vector>

using std::operator""sv;

void error(const std::string_view errorMsg)
{
    std::cout << "ERROR: " << errorMsg << '\n';
}

char to_lower(const char c)
{
    return std::tolower(static_cast<unsigned char>(c));
}

std::variant<int, const std::string_view> parse_number(const std::string_view string)
{
    std::string val;

    auto foundLetter =
        std::find_if(string.begin(), string.end(), [](auto c) { return std::isalpha(static_cast<unsigned char>(c)); });

    if (foundLetter != string.end())
    {
        return "Letters found in input."sv;
    }

    std::copy_if(string.begin(), string.end(), std::back_inserter(val),
                 [](auto c) { return std::isdigit(static_cast<unsigned char>(c)) || c == '-'; });

    int number;
    auto result = std::from_chars(val.data(), val.data() + val.size(), number);
    if (result.ec == std::errc())
    {
        return number;
    }
    else if (result.ec == std::errc::invalid_argument)
    {
        return "Invalid argument to parser";
    }
    else if (result.ec == std::errc::result_out_of_range)
    {
        return "Parsing value is too large";
    }

    return "Unknown value";
}

std::variant<int, const std::string_view> parse_number_positive(const std::string_view string)
{
    // Ensure there is no negtive number
    auto pos = string.find('-');
    if (pos != std::string_view::npos)
    {
        return "No negative numbers allowed.";
    }

    return parse_number(string);
}

struct Position
{
    std::size_t row;
    std::size_t col;
};

struct FileData
{
    std::size_t rows;
    std::size_t cols;
    std::vector<std::int16_t> values;

    std::optional<std::int16_t> at(const Position &pos)
    {
        auto index = to_index(pos);
        // std::cout << std::format("Input: {}:{} index: {} / {}", pos.col, pos.row, index, values.size()) << '\n';
        if (index >= values.size())
        {
            return {};
        }
        return values[index];
    }

    constexpr std::size_t to_index(const Position &pos)
    {
        return pos.row * cols + pos.col;
    }
};

std::optional<Position> parse_excel_fmt(const std::string_view value)
{
    auto pos = value.find_first_of("0123456789");
    if (pos == std::string_view::npos)
    {
        error("Cant find numbers in excel format");
        return {};
    }

    auto letters = value.substr(0, pos);
    // Convert all letters to lowercase
    auto toLower = [](auto l) { return std::tolower(static_cast<unsigned char>(l)); };

    std::size_t numberLetters = letters.size();
    std::size_t col = 0;

    for (auto l : letters | std::views::transform(toLower))
    {
        col += (l - 'a') * static_cast<int>(std::pow(26, numberLetters - 1));
        --numberLetters;
    }

    // Not correct!
    /*for (const auto l : letters | std::views::transform(toLower))
    {
        col += (l - 'a') + ((numberLetters - 1) * 26);
        --numberLetters;
    }*/

    auto numbers = value.substr(pos);
    auto numb = parse_number_positive(numbers);
    if (std::holds_alternative<int>(numb))
    {
        return {{static_cast<std::size_t>(std::get<int>(numb)), static_cast<std::size_t>(col)}};
    }
    return {};
}

std::optional<Position> parse_comma_fmt(const std::string_view value)
{
    auto pos = value.find_first_of(",");
    auto colText = value.substr(0, pos);
    auto rowText = value.substr(pos + 1);

    if (colText.size() == 0 || rowText.size() == 0)
    {
        error("Invalid col row comma format");
        return {};
    }

    auto varCol = parse_number_positive(colText);
    auto varRow = parse_number_positive(rowText);

    if (std::holds_alternative<int>(varCol) && std::holds_alternative<int>(varRow))
    {
        return {{static_cast<std::size_t>(std::get<int>(varRow)), static_cast<std::size_t>(std::get<int>(varCol))}};
    }
    return {};
}

std::optional<Position> parse_row_col_format(const std::string_view value)
{
    // Check the format
    auto pos = value.find_first_of(",");
    if (pos != std::string_view::npos)
    {
        return parse_comma_fmt(value);
    }

    // else do excel reading
    return parse_excel_fmt(value);
}

std::optional<FileData> load_from_file(std::filesystem::path filename)
{
    if (!std::filesystem::exists(filename))
    {
        error("Filename not found.");
        std::cout << "Filename: " << filename << '\n';
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

void non_interactive_mode(FileData &board, const std::vector<std::string> &guesses)
{
    for (const auto &guess : guesses)
    {
        const auto decoded = parse_row_col_format(guess);
        if (decoded)
        {
            auto val = board.at(decoded.value());
            if (val)
            {

                std::cout << std::format("{} = {} ", guess, val.value());
            }
            else
            {
                std::cout << std::format("{} = OOB ", guess);
            }
        }
    }
}

void interactive_mode(FileData &board, const std::filesystem::path &filename)
{
    using std::cout;
    auto show_banner = [](const std::string_view sview) { cout << std::format("{:-^60}", sview) << '\n'; };

    auto show_help = []() {
        const char e = '\n';
        cout << "Commands:" << e;
        cout << "quit -> quits the program" << e;
        cout << "show -> shows the board. Takes subcommands" << e;
        cout << "   show board -> shows the board (default)" << e;
        cout << "   show row -> asks for row number and shows that row." << e;
        cout << "   show col -> ask for the col number and shows that col" << e;
        cout << "   show window -> asks for a range of col and row and will display it." << e;
        cout << "Otherwise: " << e;
        cout << "Excel format to look up value.\nExample: A1 or B2\nOr enter in col,row\nExample: 1,2\n";
    };

    const char e = '\n';
    show_banner("Challenge 01 Interactive");
    cout << "File Name: " << filename.filename() << e;
    show_help();

    std::string userInput;
    while (userInput != "quit"sv)
    {
        cout << "Input: ";

        // Clear white space
        std::cin >> std::ws;

        // Grab an entire line rather than a word
        std::getline(std::cin, userInput);

        // Lowercase the string
        std::ranges::transform(userInput, userInput.begin(), to_lower);

        if (userInput.starts_with("quit"sv))
            break;

        if (userInput.starts_with("show"sv))
        {

            if (userInput.find("row"sv) != std::string::npos)
            {
                if (board.cols > 30)
                {
                    cout << "Too many columns to display in this row." << e;
                    continue;
                }

                cout << "Row #: ";
                std::size_t row;
                std::cin >> row;

                cout << "Row: " << row << e;
                for (Position p{row, 0}; p.col < board.cols; ++p.col)
                {
                    auto lookUp = board.at(p);
                    if (!lookUp)
                    {
                        cout << "Invalid row. Stopping." << e;
                        break;
                    }
                    cout << std::format("{:^5}", lookUp.value());
                }
                cout << e;
            }
            else if (userInput.find("col"sv) != std::string::npos)
            {
                if (board.rows > 30)
                {
                    cout << "Too many rows to display in this column." << e;
                    continue;
                }

                cout << "Col #: ";
                std::size_t col;
                std::cin >> col;
                cout << "Col: " << col << e;

                if (col > board.cols)
                {
                    cout << "Invalid col. Stopping." << e;
                    continue;
                }

                for (Position p{0, col}; p.row < board.rows; ++p.row)
                {
                    auto lookUp = board.at(p);
                    if (!lookUp)
                    {
                        cout << "Invalid col. Stopping." << e;
                        break;
                    }
                    cout << std::format("{:^5}", lookUp.value());
                }
                cout << e;
                continue;
            }
            else if (userInput.find("window"sv) != std::string::npos)
            {
                std::string upperLeftText;
                std::string lowerRightText;
                cout << "Upper left position (excel format): ";
                std::cin >> upperLeftText;
                cout << "Lower right position (excel format): ";
                std::cin >> lowerRightText;

                auto upperLeft = parse_row_col_format(upperLeftText);
                auto lowerRight = parse_row_col_format(lowerRightText);

                if (!upperLeft || !lowerRight)
                {
                    cout << "Invalid formats entered." << e;
                    continue;
                }

                Position ful, flr;
                {
                    auto ul = upperLeft.value();
                    auto lr = lowerRight.value();
                    ful.col = std::min(ul.col, lr.col);
                    ful.row = std::min(ul.row, lr.row);
                    flr.col = std::max(ul.col, lr.col);
                    flr.row = std::max(ul.row, lr.row);
                }

                if (flr.col - ful.col > 30 || flr.row - ful.row > 30)
                {
                    cout << "Too large of a window to display. Please try a smaller window." << e;
                    continue;
                }

                for (std::size_t row = ful.row; row <= flr.row; ++row)
                {
                    for (std::size_t col = ful.col; col <= flr.col; ++col)
                    {
                        Position p{row, col};
                        auto lookUp = board.at(p);
                        if (!lookUp)
                        {
                            cout << "Invalid value Stopping." << e;
                            break;
                        }
                        cout << std::format("{:^5}", lookUp.value());
                    }
                    cout << e;
                }
                continue;
            }
            else
            {
                // Show the entire board
                for (std::size_t row = 0; row < board.rows; ++row)
                {
                    for (std::size_t col = 0; col < board.cols; ++col)
                    {
                        Position p{row, col};
                        auto lookUp = board.at(p);
                        if (!lookUp)
                        {
                            cout << "Invalid value Stopping." << e;
                            break;
                        }
                        cout << std::format("{:^5}", lookUp.value());
                    }
                    cout << e;
                }
                continue;
            }
        }
        else
        {
            auto guess = parse_row_col_format(userInput);
            if (guess)
            {
                auto val = board.at(guess.value());
                if (val)
                {
                    cout << " = " << val.value() << e;
                }
                else
                {
                    cout << "Out of bounds." << e;
                }
            }
            else
            {
                cout << "Invalid format." << e;
                show_banner("Help");
                show_help();
            }
        }
    }
}

int main(int argc, char *argv[])
{
    using namespace clipp;
    // using cout = std::cout;

    std::string filename;
    std::vector<std::string> guesses;
    bool runAutomated = false;

    auto cli = (required("--load") & value("file", filename),
                option("--guess").set(runAutomated) & values("guesses", guesses));

    auto result = parse(argc, argv, cli);

    if (not result)
    {
        std::cout << "Usage:\n" << usage_lines(cli, "challenge01");
        return 0;
    }

    auto board = load_from_file(filename);
    if (!board)
    {
        error("File could not be loaded. Quitting.");
        return -1;
    }

    if (runAutomated)
    {
        non_interactive_mode(board.value(), guesses);
        std::flush(std::cout);
        return 0;
    }
    else
    {
        interactive_mode(board.value(), filename);
        return 0;
    }
}