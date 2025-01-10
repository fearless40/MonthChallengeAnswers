#pragma once

#include <string> 
#include <compare>

struct RowCol
{
    std::uint16_t row;
    std::uint16_t col;

    auto operator<=>(const RowCol &other) const = default;

    std::string as_base26_fmt() const;

    std::string as_colrow_fmt() const;

    static RowCol from_string(std::string_view const);
};
