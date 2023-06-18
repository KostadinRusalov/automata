#include "Automata.h"
#include <fstream>
#include "File.hpp"

void Automata::setAlphabet(const Alphabet &alphabet) {
    alphabet_ = alphabet;
}

void Automata::setAlphabet(Alphabet &&alphabet) {
    alphabet_ = std::move(alphabet);
}

void Automata::addSymbol(char s) {
    alphabet_.add(s);
}

void Automata::addSymbols(const Alphabet &other) {
    for (auto s: other) {
        alphabet_.add(s);
    }
}

void Automata::removeSymbol(char s) {
    alphabet_.remove(s);
}

const Automata::Alphabet &Automata::alphabet() const {
    return alphabet_;
}

void Automata::saveAlphabetTo(std::ofstream &binaryFile) const {
    File::saveSet<char>(binaryFile, alphabet_);
}

void Automata::readAlphabetFrom(std::ifstream &binaryFile) {
    File::readSet<char>(binaryFile, alphabet_);
}

