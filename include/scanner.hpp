#pragma once
#include "json.hpp"
#include <stdexcept>
#include <charconv>

namespace FastJSON {

class Parser {
public:
    static Value parse(std::string_view src) {
        size_t idx = 0;
        skip_ws(src, idx);
        auto val = parse_value(src, idx);
        return val;
    }

private:
    static void skip_ws(std::string_view src, size_t& idx) {
        while (idx < src.size() && (src[idx] == ' ' || src[idx] == '\t' || src[idx] == '\n' || src[idx] == '\r')) {
            idx++;
        }
    }

    static Value parse_value(std::string_view src, size_t& idx) {
        skip_ws(src, idx);
        if (idx >= src.size()) return Value();

        char c = src[idx];
        if (c == '{') return parse_object(src, idx);
        if (c == '[') return parse_array(src, idx);
        if (c == '"') return parse_string(src, idx);
        if ((c >= '0' && c <= '9') || c == '-') return parse_number(src, idx);
        if (src.substr(idx, 4) == "true") { idx += 4; return Value(true); }
        if (src.substr(idx, 5) == "false") { idx += 5; return Value(false); }
        if (src.substr(idx, 4) == "null") { idx += 4; return Value(); }

        throw std::runtime_error("Malformed JSON syntax");
    }

    static Value parse_string(std::string_view src, size_t& idx) {
        idx++; // skip open quote
        size_t start = idx;
        while (idx < src.size() && src[idx] != '"') idx++;
        std::string s(src.substr(start, idx - start));
        idx++; // skip close quote
        return Value(s);
    }

    static Value parse_number(std::string_view src, size_t& idx) {
        size_t start = idx;
        if (src[idx] == '-') idx++;
        while (idx < src.size() && ((src[idx] >= '0' && src[idx] <= '9') || src[idx] == '.')) idx++;
        double val = 0.0;
        std::from_chars(src.data() + start, src.data() + idx, val);
        return Value(val);
    }

    static Value parse_array(std::string_view src, size_t& idx) {
        idx++; // skip [
        Array arr;
        skip_ws(src, idx);
        if (idx < src.size() && src[idx] == ']') { idx++; return Value(arr); }
        while (idx < src.size()) {
            arr.push_back(parse_value(src, idx));
            skip_ws(src, idx);
            if (idx < src.size() && src[idx] == ',') { idx++; continue; }
            if (idx < src.size() && src[idx] == ']') { idx++; break; }
        }
        return Value(arr);
    }

    static Value parse_object(std::string_view src, size_t& idx) {
        idx++; // skip {
        Object obj;
        skip_ws(src, idx);
        if (idx < src.size() && src[idx] == '}') { idx++; return Value(obj); }
        while (idx < src.size()) {
            skip_ws(src, idx);
            std::string key = parse_string(src, idx).as_string();
            skip_ws(src, idx);
            if (src[idx] == ':') idx++;
            Value val = parse_value(src, idx);
            obj[key] = val;
            skip_ws(src, idx);
            if (idx < src.size() && src[idx] == ',') { idx++; continue; }
            if (idx < src.size() && src[idx] == '}') { idx++; break; }
        }
        return Value(obj);
    }
};

} // namespace FastJSON
