#pragma once
#include "json.hpp"

inline std::string_view parse_string_literal(std::string_view& src) {
    if (src.empty() || src[0] != '"') return {};
    size_t end = src.find('"', 1);
    if (end == std::string_view::npos) return {};
    std::string_view result = src.substr(1, end - 1);
    src.remove_prefix(end + 1);
    return result;
}
