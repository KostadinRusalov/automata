#pragma once

#include "MyStructures/Set/Set.hpp"
#include "MyStructures/BitSubset/BitSubset.h"

class DFA;

class NDFA;

class GNFA;

class Automata {
public:
    typedef Set<char> Alphabet;
    typedef unsigned State;
    typedef Set<State> StateCollection;

protected:
    Alphabet alphabet_;

public:
    virtual ~Automata() = default;

    const Alphabet &alphabet() const;

    void setAlphabet(const Alphabet &alphabet);

    void setAlphabet(Alphabet &&alphabet);

    void addSymbol(char s);

    void addSymbols(const Alphabet &other);

    void removeSymbol(char s);

};