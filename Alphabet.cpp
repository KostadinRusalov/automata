#include "Alphabet.h"
#include "MyStructures/Algorithm.hpp"

Alphabet::Alphabet(const Vector<char> &symbols) {
    for (char s: symbols) {
        add(s);
    }
}

bool Alphabet::contains(char c) const {
    return find(symbols.begin(), symbols.end(), c) != symbols.end();
}

void Alphabet::add(char c) {
    if (!contains(c)) {
        symbols.pushBack(c);
    }
}

void Alphabet::remove(char c) {
    auto symbol = find(symbols.cbegin(), symbols.cend(), c);
    symbols.erase(symbol);
}

Vector<char>::size_type Alphabet::size() const noexcept {
    return symbols.size();
}

Vector<char>::iterator Alphabet::begin() {
    return symbols.begin();
}

Vector<char>::const_iterator Alphabet::begin() const {
    return symbols.begin();
}

Vector<char>::const_iterator Alphabet::cbegin() const {
    return symbols.cbegin();
}

Vector<char>::iterator Alphabet::end() {
    return symbols.end();
}

Vector<char>::const_iterator Alphabet::end() const {
    return symbols.end();
}

Vector<char>::const_iterator Alphabet::cend() const {
    return symbols.cend();
}
