#pragma once
namespace parser::util
{

constexpr char to_lower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c + 'a' - 'A';
    return c;
}

} // namespace parser::util