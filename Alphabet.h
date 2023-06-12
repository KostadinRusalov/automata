#pragma once

#include "MyStructures/Vector/Vector.hpp"


class Alphabet {
private:
    Vector<char> symbols;

public:
    Alphabet() = default;

    explicit Alphabet(const Vector<char>& symbols);

    void add(char c);
};