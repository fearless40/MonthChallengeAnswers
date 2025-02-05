
#include "commandline.hpp"
#include "createmode.hpp"
#include "parser.hpp"
#include "verifymode.hpp"
#include <iostream>

int main(int argc, char *argv[])
{

    int value = 0;
    auto caller_me = [](std::string_view view) -> void { std::cout << "From call back: " << view << '\n'; };
    std::string_view letters;
    auto digit = parser::dsl::int_parser<int> >> parser::dsl::letters_ignore_case;
    std::string_view test = "123   abc";
    parser::parse(test, digit, parser::white_space_not_endl{});
    std::cout << '\n' << "Found: " << value << " and " << letters << '\n';

    return 0;
    /*auto opt = commandline::parse(argc, argv);

    switch (opt.mode)
    {
    case commandline::RunMode::Create:
        return run_command_mode_create(opt);

    case commandline::RunMode::Verify:
        return run_command_mode_verify(opt);
    }*/
}