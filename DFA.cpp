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
//    transitions.erase(transitions.cbegin() + state);
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

    finalStates.pushBack(final);
    return final;
}

void DFA::makeFinalState(DFA::State state) {
    if (!isValid(state)) {
        throw std::logic_error(INVALID_STATE);
    }
    finalStates.pushBack(state);
}

void DFA::removeFinalState(DFA::State state) {
    if (!isValid(state)) {
        throw std::logic_error(INVALID_STATE);
    }

    auto pos = find(finalStates.cbegin(), finalStates.cend(), state);
    if (pos != finalStates.cend()) {
        finalStates.erase(pos);
    }
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
    auto pos = find(st.cbegin(), st.cend(), transition);

    if (pos != st.cend()) {
        st.erase(pos);
    }
}

bool DFA::isTotal() const {
    return allOf(transitions.begin(), transitions.end(),
                 [&](const Vector<Transition> &tr) {
                     return tr.size() == alphabet.size();
                 });
}

void DFA::makeTotal() {
    if (isTotal()) {
        return;
    }

    transitions.pushBack({});
    State dump = transitions.size() - 1;

    for (State s = 0; s < transitions.size(); ++s) {
        Vector<Transition> &stateTr = transitions[s];

        if (stateTr.size() < alphabet.size()) {
            Alphabet leftSymbols(alphabet);

            for (auto &tr: stateTr) {
                leftSymbols.remove(tr.first);
            }

            for (char symbol: leftSymbols) {
                addTransition(s, symbol, dump);
            }
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

    return find(finalStates.begin(), finalStates.end(), next) != finalStates.end();
}

void DFA::setAlphabet(const Alphabet &alphabet) {
    this->alphabet = alphabet;
}

void DFA::setAlphabet(Alphabet &&alphabet) {
    this->alphabet = std::move(alphabet);
}


