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

void DFA::removeState(DFA::State state) {
//    if (!isValid(state)) {
//        throw std::logic_error(INVALID_STATE);
//    }
//    if (state == initialState) {
//        initialState = transitions.size() - 1;
//    }
//
//    finalStates.remove(state);
//    Algorithm::transform(finalStates.begin(), finalStates.end(), finalStates.begin(),
//                         [state](State curr) {
//                             return curr > state ? curr - 1 : curr;
//                         });
//
//    transitions.erase(transitions.cbegin() + state);
//    for (auto &stateTr: transitions) {
//        Vector<Transition> toErase(stateTr.size());
//        for (auto &tr: stateTr) {
//            if (tr.second == state) {
//                toErase.pushBack(tr);
//            }
//        }
//        for (auto &tr: toErase) {
//
//        }
//    }
}

DFA::State DFA::addInitialState() {
    transitions.pushBack({});
    initialState = lastState();
    return initialState;
}

void DFA::makeInitialState(DFA::State state) {
    if (!isValid(state)) {
        throw std::logic_error(INVALID_STATE);
    }
    initialState = state;
}


DFA::State DFA::addFinalState() {
    transitions.pushBack({});
    State final = lastState();

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

void DFA::addSymbol(char s) {
    alphabet.add(s);
}

void DFA::removeSymbol(char s) {
    alphabet.remove(s);
}

void DFA::addTransition(DFA::State from, char with, DFA::State to) {
    if (!isValid(from) || !isValid(to)) {
        throw std::logic_error(INVALID_STATE);
    }

    if (!alphabet.contains(with)) {
        throw std::logic_error(INVALID_SYMBOL);
    }

    Vector<Transition> &stateTransitions = transitions[from];
    for (Transition &transition: stateTransitions) {
        if (with == transition.first) {
            transition.second = to;
            return;
        }
    }
    stateTransitions.pushBack({with, to});
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
    auto pos = Algorithm::find(st.cbegin(), st.cend(), transition);

    if (pos != st.cend()) {
        st.erase(pos);
    }
}

bool DFA::isTotal() const {
    return Algorithm::allOf(transitions.begin(), transitions.end(),
                            [&](const Vector<Transition> &tr) {
                                return tr.size() == alphabet.size();
                            });
}

void DFA::makeTotal() {
    if (isTotal()) {
        return;
    }

    transitions.pushBack({});
    State dump = lastState();

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

DFA::State DFA::errorState() const {
    return transitions.size();
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

void DFA::setAlphabet(const Alphabet &alphabet) {
    this->alphabet = alphabet;
}

void DFA::setAlphabet(Alphabet &&alphabet) {
    this->alphabet = std::move(alphabet);
}


