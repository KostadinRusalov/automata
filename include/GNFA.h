#pragma once

#include "Automata.h"
#include "Regex.h"

class GNFA : public Automata {
public:
    typedef Pair<State, String> Transition;
    typedef Pair<State, StringView> TempTransition;
    typedef Vector<TempTransition> TempTransitions;
    typedef Vector<Vector<Transition>> TransitionCollection;
    typedef Vector<Transition>::const_iterator transitionIterator;

private:
    State initialState;
    State finalState;
    TransitionCollection transitions;

    State lastState() const;

    TempTransitions findRightStates(State state) const;

    TempTransitions findLeftStates(State state) const;

    static String mergedRegexes(const StringView& right, const StringView& left, const String& loop) ;

    void ripState(State rip);

public:
    State addState();

    void removeState(State state);

    void addInitialState();

    void addFinalState();

    void addTransition(State from, State to, const String &regex);

    static GNFA convertFrom(const DFA &dfa);

    String convertToRegex() const;

    transitionIterator findTransition(State from, State to) const;

    friend void print(const GNFA &gnfa);
};