#pragma once

#include "DFA.h"
#include "CState.hpp"

// constructional DFA
template<class T>
class CDFA : public DFA {
public:
    typedef CState<T> CState;
    typedef Vector<CState> CStateCollection;

    typedef typename CStateCollection::const_iterator const_iterator;

protected:
    CStateCollection cStates;


public:
    State addState() = delete;

    State addInitialState() = delete;

    State addFinalState() = delete;

    State addState(const CState &cState);

    State addState(CState &&cState);

    void removeState(State state);

    State addInitialState(const CState &cState);

    State addInitialState(CState &&cState);

    State addFinalState(const CState &cState);

    State addFinalState(CState &&cState);

    void addTransition(const CState &from, char with, const CState &to);

    void removeTransition(const CState &from, char with, const CState &to);

    State findState(const CState &cState) const;

    const_iterator findCState(State state) const;
};

template<class T>
DFA::State CDFA<T>::addState(const CState &cState) {
    cStates.pushBack(cState);
    return DFA::addState();

}

template<class T>
DFA::State CDFA<T>::addState(CState &&cState) {
    cStates.pushBack(std::move(cState));
    return DFA::addState();

}

template<class T>
void CDFA<T>::removeState(DFA::State state) {
    DFA::removeState(state);
    cStates.erase(cStates.cbegin() + state);
}

template<class T>
DFA::State CDFA<T>::addInitialState(const CState &cState) {
    cStates.pushBack(cState);
    return DFA::addInitialState();
}

template<class T>
DFA::State CDFA<T>::addInitialState(CState &&cState) {
    cStates.pushBack(std::move(cState));
    return DFA::addInitialState();
}

template<class T>
DFA::State CDFA<T>::addFinalState(const CState &cState) {
    cStates.pushBack(cState);
    return DFA::addFinalState();
}

template<class T>
DFA::State CDFA<T>::addFinalState(CState &&cState) {
    cStates.pushBack(std::move(cState));
    return DFA::addFinalState();
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

template<class T>
DFA::State CDFA<T>::findState(const CState &cState) const {
    auto it = kstd::find(cStates.begin(), cStates.end(), cState);
    return it - cStates.begin();
}

template<class T>
typename CDFA<T>::const_iterator CDFA<T>::findCState(DFA::State state) const {
    if (state >= cStates.size()) {
        return cStates.end();
    }
    return cStates.begin() + state;
}