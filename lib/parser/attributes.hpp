#pragma once
#include <tuple>

namespace parser::attributes
{

struct parser_tag
{
};

struct ws_never
{
};

struct ws_always
{
};

template <class... T> using Attributes = std::tuple<T...>;

} // namespace parser::attributes
