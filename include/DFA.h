#pragma once

#include "../MyStructures/Pair/Pair.hpp"
#include "Automata.h"
#include "../MyStructures/StringView/StringView.h"

class DFA : public Automata {
public:
    typedef Pair<char, State> Transition;
    typedef Vector<Vector<Transition>> TransitionCollection;

    friend class GNFA;

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

    static Vector<Transition>::const_iterator findTransition(const Vector<Transition> &stateTr, char with);

public:
    DFA() = default;

    explicit DFA(const Alphabet &alphabet);

    explicit DFA(Alphabet &&alphabet);

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

    DFA total() const;

    void removeUnreachableStates();

    bool accepts(const char *word) const;

    NDFA reversed() const;

    DFA minimized() const;

    bool isEmptyLanguage() const;

    DFA operator!() const;

    void saveTo(std::ofstream &binaryFile) const;

    static DFA readFrom(std::ifstream &binaryFile);

    friend DFA operator&(const DFA &rhs, const DFA &lhs);

    friend void print(const DFA &dfa);
};

DFA operator|(const DFA &rhs, const DFA &lhs);

DFA operator/(const DFA &rhs, const DFA &lhs);

DFA operator&(const DFA &rhs, const DFA &lhs);