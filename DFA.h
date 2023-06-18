#pragma once

#include "MyStructures/Pair/Pair.hpp"
#include "Automata.h"

class DFA : public Automata {
public:
    typedef Pair<char, State> Transition;
    typedef Vector<Vector<Transition>> TransitionCollection;

protected:
    State initialState;
    StateCollection finalStates;
    TransitionCollection transitions;

    State lastState() const;

    State nextState(State current, char s) const;

    void validate(State state) const;

    void validate(char s) const;

    void findReachableStates(State from, BitSubset &reachable) const;

    BitSubset unreachableStates() const;

public:
    State errorState() const;

    State addState();

    void removeState(State state);

    State addInitialState();

    void makeInitialState(State state);

    State addFinalState();

    void makeFinalState(State state);

    void removeFinalState(State state);

    void addTransition(State from, char with, State to);

    void removeTransition(State from, char with, State to);

    bool isTotal() const;

    void makeTotal();

    void removeUnreachableStates();

    bool accepts(const char *word) const;

    NDFA reversed() const;

    friend void print(const DFA &dfa);
};