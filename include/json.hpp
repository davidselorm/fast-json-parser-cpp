#pragma once
#include <string_view>
#include <vector>

enum class JsonType { Null, Bool, Number, String, Array, Object };

struct JsonValue {
    JsonType type;
    std::string_view raw;
};
