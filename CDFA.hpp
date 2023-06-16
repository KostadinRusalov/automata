#pragma once

#include "DFA.h"
#include "CState.hpp"

// constructional DFA
template<class T>
class CDFA : public DFA {
public:
    typedef CState<T> CState;
    typedef Vector<CState> CStateCollection;

protected:
    CStateCollection cStates;

    State findState(const CState &cState) const;

public:
    State addState() override = delete;

    State addInitialState() override = delete;

    State addFinalState() override = delete;

    State addState(const CState &cState);

    State addState(CState &&cState);

    void removeState(State state);

    void addInitialState(const CState &cState);

    void addInitialState(CState &&cState);

    void addFinalState(const CState &cState);

    void addFinalState(CState &&cState);

    void addTransition(const CState &from, char with, const CState &to);

    void removeTransition(const CState &from, char with, const CState &to);
};

template<class T>
Automata::State CDFA<T>::addState(const CState &cState) {
    DFA::addState();
    cStates.pushBack(cState);
}

template<class T>
Automata::State CDFA<T>::addState(CState &&cState) {
    DFA::addState();
    cStates.pushBack(std::move(cState));
}

template<class T>
void CDFA<T>::removeState(DFA::State state) {
    DFA::removeState(state);
    cStates.erase(cStates.cbegin() + state);
}

template<class T>
void CDFA<T>::addInitialState(const CState &cState) {
    DFA::addInitialState();
    cStates.pushBack(cState);
}

template<class T>
void CDFA<T>::addInitialState(CState &&cState) {
    DFA::addInitialState();
    cStates.pushBack(std::move(cState));
}

template<class T>
void CDFA<T>::addFinalState(const CState &cState) {
    DFA::addFinalState();
    cStates.pushBack(cState);
}

template<class T>
void CDFA<T>::addFinalState(CState &&cState) {
    DFA::addFinalState();
    cStates.pushBack(std::move(cState));
}

template<class T>
DFA::State CDFA<T>::findState(const CState &cState) const {
    auto it = kstd::find(cStates.begin(), cStates.end(), cStates);
    return it - cStates.begin();
}

template<class T>
void CDFA<T>::addTransition(const CState &from, char with, const CState &to) {
    auto from_ = findState(from);
    auto to_ = findState(to);
    DFA::addTransition(from_, with, to_);
}

template<class T>
void CDFA<T>::removeTransition(const CState &from, char with, const CState &to) {
    auto from_ = findState(from);
    auto to_ = findState(to);
    DFA::removeTransition(from_, with, to_);
}