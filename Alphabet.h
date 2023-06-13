#pragma once

#include "MyStructures/Vector/Vector.hpp"


class Alphabet {
private:
    Vector<char> symbols;

public:
    Alphabet() = default;

    Alphabet(const Vector<char> &symbols);

    bool contains(char c) const;

    void add(char c);

    void remove(char c);
};