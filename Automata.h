#pragma once

#include "Alphabet.h"
#include "State.hpp"

template<class T>
class Automata {
protected:
    Alphabet alphabet;
    Vector<State<T>> states;
    Vector<State<T> *> finalStates;

public:

    virtual ~Automata() = default;
};