#include "Automata.h"

void Automata::setAlphabet(const Alphabet &alphabet) {
    this->alphabet = alphabet;
}

void Automata::setAlphabet(Alphabet &&alphabet) {
    this->alphabet = std::move(alphabet);
}

void Automata::addSymbol(char s) {
    alphabet.add(s);
}

void Automata::removeSymbol(char s) {
    alphabet.remove(s);
}