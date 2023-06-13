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
