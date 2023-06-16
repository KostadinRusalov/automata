#pragma once

#include "Automata.h"
#include "MyStructures/Pair/Pair.hpp"
#include "CDFA.hpp"

class NDFA : public Automata {
public:
    typedef Pair<char, Set<State>> Transition;
    typedef Vector<Vector<Transition>> TransitionCollection;

    friend class NDFAFactory;

protected:
    StateCollection initialStates;
    StateCollection finalStates;
    TransitionCollection transitions;

    State lastState() const;

    State errorState() const;

    void validate(State state) const;

    void validate(char s) const;

    static Vector<Transition>::const_iterator findTransition(const Vector<Transition> &stateTr, char with);

    int accepts(State from, const char *word) const;

    static State offset(State state, State offsetIdx);

    void copyTransitions(State state, const Vector<Transition> &stateTr, State offsetIdx);

    Subset unreachableStates() const;

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

    void removeUnreachableStates();

    bool accepts(const char *word) const;

    NDFA &operator+=(const NDFA &other);

    NDFA &operator|=(const NDFA &other);

    NDFA &operator*=(const NDFA &other);

    NDFA operator*();

    DFA getDeterminized() const;

    friend void print(const NDFA &ndfa);
};

NDFA operator+(const NDFA &rhs, const NDFA &lhs);

NDFA operator|(const NDFA &rhs, const NDFA &lhs);

NDFA operator*(const NDFA &rhs, const NDFA &lhs);
