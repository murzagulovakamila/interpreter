#pragma once

#include <memory>

#include "type.hpp"

enum class Types {
	DEFAULT, INT, DOUBLE, STRING, BOOL
};
struct Value {

    std::shared_ptr<Type> type;
    Types types;
    virtual ~Value() noexcept = default;
};


struct Rvalue : public Value {
    virtual ~Rvalue() noexcept = default;
};

struct IntValue : public Rvalue {
    int value;
};

struct FloatValue : public Rvalue {
    float value;
};

struct CharValue : public Rvalue {
    char value;
};

struct BoolValue : public Rvalue {
    bool value;
};

struct PointerValue : public Rvalue {
    unsigned long long value;
};
struct StringValue : public Rvalue {
	std::string value;
};

struct Lvalue : public Value {
    std::shared_ptr<Rvalue> value;
};
