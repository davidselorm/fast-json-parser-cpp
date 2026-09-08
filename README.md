# fast-json-parser-cpp

High-throughput zero-copy JSON parser and tokenizer written in modern C++20.

## Features
- **String View Tokenization**: Avoids string heap allocations during scanning.
- **Fast Numeric Conversions**: Uses `std::from_chars` for locale-independent float parsing.
