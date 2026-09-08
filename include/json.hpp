#pragma once
#include <string_view>
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <memory>

namespace FastJSON {

enum class Type { Null, Boolean, Number, String, Array, Object };

struct Value;
using Object = std::unordered_map<std::string, Value>;
using Array = std::vector<Value>;

struct Value {
    Type type;
    std::variant<std::nullptr_t, bool, double, std::string, Array, Object> data;

    Value() : type(Type::Null), data(nullptr) {}
    Value(bool b) : type(Type::Boolean), data(b) {}
    Value(double d) : type(Type::Number), data(d) {}
    Value(std::string s) : type(Type::String), data(std::move(s)) {}
    Value(Array a) : type(Type::Array), data(std::move(a)) {}
    Value(Object o) : type(Type::Object), data(std::move(o)) {}

    bool is_number() const { return type == Type::Number; }
    double as_number() const { return std::get<double>(data); }
    const std::string& as_string() const { return std::get<std::string>(data); }
};

} // namespace FastJSON
