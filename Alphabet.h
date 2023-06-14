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

    Vector<char>::size_type size() const noexcept;

    Vector<char>::iterator begin();

    Vector<char>::const_iterator begin() const;

    Vector<char>::const_iterator cbegin() const;

    Vector<char>::iterator end();

    Vector<char>::const_iterator end() const;

    Vector<char>::const_iterator cend() const;
};