#pragma once

#include "MyStructures/OrderedSet/OrderedSet.hpp"

class Automata {
public:
    typedef OrderedSet<char> Alphabet;
    typedef unsigned State;
    typedef OrderedSet<State> StateCollection;

protected:
    Alphabet alphabet;

public:
    virtual ~Automata() = default;

    void setAlphabet(const Alphabet &alphabet);

    void setAlphabet(Alphabet &&alphabet);

    void addSymbol(char s);

    void addSymbols(const Alphabet &other);

    void removeSymbol(char s);
};