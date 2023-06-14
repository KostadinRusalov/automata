#include "NDFA.h"
#include "SubtractOneAfter.hpp"

const char INVALID_STATE[] = "There is no such state in the DFA!";
const char INVALID_SYMBOL[] = "There is no such symbol in the alphabet!";

bool NDFA::isValid(NDFA::State state) const {
    return state < transitions.size();
}

NDFA::State NDFA::lastState() const {
    return transitions.size() - 1;
}

NDFA::State NDFA::errorState() const {
    return transitions.size();
}

NDFA::State NDFA::addState() {
    transitions.pushBack({});
    return lastState();
}

void NDFA::validate(State state) const {
    if (!isValid(state)) {
        throw std::logic_error(INVALID_STATE);
    }
}

void NDFA::removeState(NDFA::State state) {
    validate(state);

    removeAndShift(state, initialStates);
    removeAndShift(state, finalStates);

    transitions.erase(transitions.cbegin() + state);
    for (auto &stateTr: transitions) {
        for (auto &tr: stateTr) {
            removeAndShift(state, tr.second);
        }
    }
}

NDFA::State NDFA::addInitialState() {
    State initial = addState();
    initialStates.add(initial);
    return initial;
}

void NDFA::makeInitialState(Automata::State state) {
    validate(state);
    initialStates.add(state);
}

void NDFA::removeInitialState(NDFA::State state) {
    validate(state);
    initialStates.remove(state);
}

NDFA::State NDFA::addFinalState() {
    State final = addState();
    finalStates.add(final);
    return final;
}

void NDFA::makeFinalState(NDFA::State state) {
    validate(state);
    finalStates.add(state);
}

void NDFA::removeFinalState(NDFA::State state) {
    validate(state);
    finalStates.remove(state);
}

typename Vector<NDFA::Transition>::const_iterator
NDFA::findTransition(const Vector<Transition> &stateTr, char with) {
    return kstd::findIf(stateTr.begin(), stateTr.end(),
                        [with](const Transition &tr) {
                            return tr.first == with;
                        });
}

void NDFA::addTransition(NDFA::State from, char with, NDFA::State to) {
    // TODO validation
    auto &stateTr = transitions[from];
    // TODO const_iterator to iterator
    auto tr = stateTr.begin() + (findTransition(stateTr, with) - stateTr.cbegin());

    if (tr == stateTr.end()) {
        stateTr.pushBack({with, {to}});
    } else {
        tr->second.add(to);
    }
}

void NDFA::removeTransition(NDFA::State from, char with, NDFA::State to) {
    // TODO validation
    auto &stateTr = transitions[from];
    auto tr = stateTr.begin() + (findTransition(stateTr, with) - stateTr.cbegin());

    if (tr != stateTr.end()) {
        tr->second.remove(to);
    }
}

bool NDFA::isTotal() const {
    return kstd::allOf(
            transitions.begin(), transitions.end(),
            [&](const Vector<Transition> &tr) {
                return tr.size() == alphabet.size();
            }
    );
}

void NDFA::makeTotal() {
    if (isTotal()) { return; }

    State dump = addState();
    for (State s = 0; s < transitions.size(); ++s) {
        auto &stateTr = transitions[s];
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

int NDFA::accepts(NDFA::State from, const char *word) const {
    if (*word == '\0') {
        return finalStates.contains(from);
    }

    auto &stateTr = transitions[from];
    char next = *word;
    auto tr = findTransition(stateTr, next);

    if (tr == stateTr.end()) {
        return 0;
    }

    int sum = 0;
    ++word;
    for (State state: tr->second) {
        sum += accepts(state, word);
        if (sum >= 1) {
            return sum;
        }
    }
    return sum;
}

bool NDFA::accepts(const char *word) const {
    int sum = 0;
    for (State initial: initialStates) {
        sum += accepts(initial, word);
        if (sum >= 1) {
            return true;
        }
    }
    return false;
}

NDFA &NDFA::operator+=(const NDFA &other) {
    size_t offsetCount = transitions.size();

    auto offset = [offsetCount](State state) {
        return state + offsetCount;
    };

    for (auto &stateTr: other.transitions) {
        auto newState = addState();

        for (auto &tr: stateTr) {
            for (auto &st: tr.second) {
                addTransition(newState, tr.first, offset(st));
            }
        }
    }

    return *this;
}

NDFA &NDFA::operator|=(const NDFA &other) {
    // TODO ednkakva azbuka ?
    size_t offsetCount = transitions.size();
    auto offset = [offsetCount](State state) {
        return state + offsetCount;
    };
    operator+=(other);

    for (auto initial: other.initialStates) {
        makeInitialState(offset(initial));
    }

    for (auto final: other.finalStates) {
        makeFinalState(offset(final));
    }
    return *this;
}

NDFA &NDFA::operator*=(const NDFA &other) {
    size_t offsetCount = transitions.size();
    auto offset = [offsetCount](State state) {
        return state + offsetCount;
    };
    operator+=(other);

    for (auto final: finalStates) {
        for (auto initial: other.initialStates) {
            for (auto &tr: other.transitions[initial]) {
                for (auto st: tr.second) {
                    addTransition(final, tr.first, offset(st));
                }
            }
        }
    }

    auto it = kstd::findIf(other.initialStates.begin(),
                           other.initialStates.end(),
                           [&other](State state) {
                               return other.finalStates.contains(state);
                           });

    if (it == other.initialStates.end()) {
        for (auto final: finalStates) {
            finalStates.remove(final);
        }
    }

    for (auto final: other.finalStates) {
        makeFinalState(offset(final));
    }
    return *this;
}

NDFA NDFA::operator*() {
    NDFA n(*this);

    n.initialStates.clear();
    n.makeFinalState(n.addInitialState());

    for (auto final: n.finalStates) {
        for (auto initial: initialStates) {
            for (auto &tr: transitions[initial]) {
                for (auto st: tr.second) {
                    n.addTransition(final, tr.first, st);
                }
            }
        }
    }
    return n;
}


NDFA operator|(const NDFA &rhs, const NDFA &lhs) {
    NDFA n(rhs);
    n |= lhs;
    return n;
}

NDFA operator*(const NDFA &rhs, const NDFA &lhs) {
    NDFA n(rhs);
    n *= lhs;
    return n;
}