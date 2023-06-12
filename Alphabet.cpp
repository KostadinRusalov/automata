#include "Alphabet.h"
#include "MyStructures/Algorithm.hpp"

Alphabet::Alphabet(const Vector<char> &symbols)
        : symbols(symbols) {}

void Alphabet::add(char c) {
    if (find(symbols.begin(), symbols.end(), c) == symbols.end()) {
        symbols.pushBack(c);
    }
}
