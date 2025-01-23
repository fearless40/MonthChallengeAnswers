
#include "commandline.hpp"
#include "createmode.hpp"
#include "verifymode.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    auto opt = commandline::parse(argc, argv);

    switch (opt.mode)
    {
    case commandline::RunMode::Create:
        return run_command_mode_create(opt);

    case commandline::RunMode::Verify:
        return run_command_mode_verify(opt);
    }
}