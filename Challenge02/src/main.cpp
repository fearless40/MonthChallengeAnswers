
#include "commandline.hpp"
#include "createmode.hpp"
#include "dsl.hpp"
#include "parser.hpp"
#include "verifymode.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    /*namespace dsl = parser::dsl;
    int value = 0;
    auto caller_me = [](std::string_view view) -> void { std::cout << "From call back: " << view << '\n'; };
    std::string_view letters;
    auto digit = dsl::int_parser<int>(value) >> dsl::letters_ignore_case(letters);
    std::string_view test = "123   abc";
    auto excel_fmt = (dsl::letters_ignore_case + dsl::int_parser<std::uint16_t>)(letters) >>
                     dsl::letters_ignore_case([](auto str) { std::cout << "And found " << str << '\n'; });
    parser::parse(test, digit, dsl::white_space_not_endl);
    std::cout << '\n' << "Found: " << value << " and " << letters << '\n';
    std::string_view test2 = "BA14    abcdefg";
    parser::parse(test2, excel_fmt, dsl::white_space_not_endl);
    std::cout << "Matched: " << letters << '\n';

    auto test_sep = dsl::letters_ignore_case(caller_me) >> dsl::single_char<';'> >> dsl::letters_ignore_case(caller_me);
    std::string_view test_sep_string = "abc;efg";
    parser::parse(test_sep_string, test_sep, dsl::white_space_not_endl);

    return 0;*/

    auto opt = commandline::parse(argc, argv);

    switch (opt.mode)
    {
    case commandline::RunMode::Create:
        return run_command_mode_create(opt);

    case commandline::RunMode::Verify:
        return run_command_mode_verify(opt);
    }
}
