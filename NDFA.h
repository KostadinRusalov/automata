#pragma once

#include "Automata.h"
#include "MyStructures/Pair/Pair.hpp"

class NDFA : public Automata {
public:
    typedef Pair<char, Set<State>> Transition;
    typedef Vector<Vector<Transition>> TransitionCollection;

    friend class NDFAFactory;

protected:
    StateCollection initialStates;
    StateCollection finalStates;
    TransitionCollection transitions;

private:
    bool isValid(State state) const;

    State lastState() const;

    State errorState() const;

    int accepts(State from, const char *word) const;

    NDFA &operator+=(const NDFA &other);

public:
    // State logic
    State addState();

    void removeState(State state);

    State addInitialState();

    void makeInitialState(State state);

    void removeInitialState(State state);

    State addFinalState();

    void makeFinalState(State state);

    void removeFinalState(State state);

    // Transition logic
    void addTransition(State from, char with, State to);

    void removeTransition(State from, char with, State to);

    // other
    bool isTotal() const;

    void makeTotal();

    bool accepts(const char *word) const;

    NDFA &operator|=(const NDFA &other);

    NDFA &operator*=(const NDFA &other);

    NDFA operator*();

    friend void print(const NDFA &ndfa);
};

NDFA operator|(const NDFA &rhs, const NDFA &lhs);

NDFA operator*(const NDFA &rhs, const NDFA &lhs);
