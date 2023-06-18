#pragma once

#include "Automata.h"
#include "Regex.h"

class GNFA : public Automata {
public:
    typedef Pair<Regex, State> Transition;
    typedef Vector<Vector<Transition>> TransitionCollection;
    typedef Vector<Transition>::const_iterator transitionIterator;
protected:
    State initialState;
    State finalState;
    TransitionCollection transitions;

    State lastState() const;

public:

    State addState();

    void addInitialState();

    void addFinalState();

    void addTransition(State from, const Regex &with, State to);

    static GNFA convertFrom(const DFA &dfa);

    Regex convertToRegex() const;

    transitionIterator findTransition(State from, State to) const;

    Regex findRegex(State from, State to) const;

    friend void print(const GNFA &gnfa);

};