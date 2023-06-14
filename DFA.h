#pragma once

#include "Automata.h"
#include "MyStructures/Pair/Pair.hpp"
#include "MyStructures/String/String.h"

class DFA : public Automata {
public:
    typedef unsigned State;
    typedef Vector<State> StateCollection;
    typedef Pair<char, State> Transition;
    typedef Vector<Vector<Transition>> TransitionCollection;

protected:
    State initialState;
    StateCollection finalStates;
    TransitionCollection transitions;
private:
    bool isValid(State state) const;

    State lastState() const;

    State nextState(State current, char s) const;

    State errorState() const;

public:

    State addState();

    void removeState(State state);

    State addInitialState();

    void makeInitialState(State state);

    State addFinalState();

    void makeFinalState(State state);

    void removeFinalState(State state);

    void setAlphabet(const Alphabet &alphabet);

    void setAlphabet(Alphabet &&alphabet);

    void addSymbol(char s);

    void removeSymbol(char s);

    void addTransition(State from, char with, State to);

    void removeTransition(State from, char with, State to);

    bool isTotal() const;

    void makeTotal();

    void removeUnreachableStates();

    bool accepts(const char *word) const;

//    void saveToFile();
//
//    void loadFromFile();


};