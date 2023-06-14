#include "DFA.h"
#include "MyStructures/Algorithm.hpp"

const char INVALID_STATE[] = "There is no such state in the DFA!";
const char INVALID_SYMBOL[] = "There is no such symbol in the alphabet!";

bool DFA::isValid(DFA::State state) const {
    return state < transitions.size();
}

DFA::State DFA::lastState() const {
    return transitions.size() - 1;
}

DFA::State DFA::addState() {
    transitions.pushBack({});
    return lastState();
}

DFA::State DFA::errorState() const {
    return transitions.size();
}

void DFA::removeState(DFA::State state) {
    if (!isValid(state)) {
        throw std::logic_error(INVALID_STATE);
    }

    if (state == initialState) {
        initialState = errorState();
    }

    finalStates.remove(state);
    kstd::transform(
            finalStates.begin(), finalStates.end(),
            finalStates.begin(),
            [state](State curr) {
                return curr > state ? curr - 1 : curr;
            }
    );

    transitions.erase(transitions.cbegin() + state);
    for (auto &stateTr: transitions) {
        for (size_t i = 0; i < stateTr.size(); ++i) {
            if (stateTr[i].second == state) {
                stateTr.erase(stateTr.cbegin() + i);
                --i;
            } else if (stateTr[i].second > state) {
                --stateTr[i].second;
            }
        }
    }
}

DFA::State DFA::addInitialState() {
    initialState = addState();
    return initialState;
}

void DFA::makeInitialState(DFA::State state) {
    if (!isValid(state)) {
        throw std::logic_error(INVALID_STATE);
    }
    initialState = state;
}


DFA::State DFA::addFinalState() {
    State final = addState();
    finalStates.add(final);
    return final;
}

void DFA::makeFinalState(DFA::State state) {
    if (!isValid(state)) {
        throw std::logic_error(INVALID_STATE);
    }
    finalStates.add(state);
}

void DFA::removeFinalState(DFA::State state) {
    if (!isValid(state)) {
        throw std::logic_error(INVALID_STATE);
    }
    finalStates.remove(state);
}

void DFA::addTransition(DFA::State from, char with, DFA::State to) {
    if (!isValid(from) || !isValid(to)) {
        throw std::logic_error(INVALID_STATE);
    }

    if (!alphabet.contains(with)) {
        throw std::logic_error(INVALID_SYMBOL);
    }

    Vector<Transition> &stateTr = transitions[from];
    auto tr = kstd::findIf(stateTr.begin(), stateTr.end(),
                           [with](const Transition &tr) {
                               return tr.first == with;
                           });
    if (tr == stateTr.end()) {
        stateTr.pushBack({with, to});
    } else {
        tr->second = to;
    }
}

void DFA::removeTransition(DFA::State from, char with, DFA::State to) {
    if (!isValid(from) || !isValid(to)) {
        throw std::logic_error(INVALID_STATE);
    }

    if (!alphabet.contains(with)) {
        throw std::logic_error(INVALID_SYMBOL);
    }

    Vector<Transition> &st = transitions[from];
    Transition transition(with, to);
    auto pos = kstd::find(st.cbegin(), st.cend(), transition);

    if (pos != st.cend()) {
        st.erase(pos);
    }
}

bool DFA::isTotal() const {
    return kstd::allOf(transitions.begin(), transitions.end(),
                       [&](const Vector<Transition> &tr) {
                           return tr.size() == alphabet.size();
                       });
}

void DFA::makeTotal() {
    if (isTotal()) {
        return;
    }

    State dump = addState();

    for (State s = 0; s < transitions.size(); ++s) {
        Vector<Transition> &stateTr = transitions[s];
        if (stateTr.size() == alphabet.size()) {
            continue;
        }
        Alphabet leftSymbols(alphabet);

        for (auto &tr: stateTr) {
            leftSymbols.remove(tr.first);
        }

        for (char symbol: leftSymbols) {
            addTransition(s, symbol, dump);
        }
    }
}

void DFA::removeUnreachableStates() {

}


DFA::State DFA::nextState(DFA::State current, char s) const {
    auto &stateTr = transitions[current];
    for (auto &tran: stateTr) {
        if (tran.first == s) {
            return tran.second;
        }
    }
    return errorState();
}

bool DFA::accepts(const char *word) const {
    State next = initialState;

    while (*word && next != errorState()) {
        next = nextState(next, *word);
        ++word;
    }

    if (next == errorState()) {
        return false;
    }

    return finalStates.contains(next);
}


