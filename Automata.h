#pragma once

#include "MyStructures/OrderedSet/OrderedSet.hpp"

class Automata {
public:
    typedef OrderedSet<char> Alphabet;
public:
    virtual ~Automata() = default;
};