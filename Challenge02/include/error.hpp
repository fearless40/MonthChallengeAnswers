#pragma once
#include <string>
#include <vector>

struct ErrorReport
{
    std::vector<std::string> errors;

    void operator()(std::string error)
    {
        errors.emplace_back(error);
    }

    operator bool() const
    {
        return has_errors();
    }

    bool has_errors() const
    {
        return errors.size() != 0;
    }
};
