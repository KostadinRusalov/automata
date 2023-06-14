#include "NDFA.h"

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

void NDFA::removeState(NDFA::State state) {
    if (!isValid(state)) {
        throw std::logic_error(INVALID_STATE);
    }

    initialStates.remove(state);
    finalStates.remove(state);

    auto subtractOne = [state](State curr) {
        return curr > state ? curr - 1 : curr;
    };

    kstd::transform(initialStates.begin(), initialStates.end(),
                         initialStates.begin(), subtractOne);
    kstd::transform(finalStates.begin(), finalStates.end(),
                         finalStates.begin(), subtractOne);

    transitions.erase(transitions.cbegin() + state);

    for (auto &stateTr: transitions) {
        for (auto &tr: stateTr) {
            tr.second.remove(state);
            kstd::transform(tr.second.begin(), tr.second.end(),
                                 tr.second.begin(), subtractOne);
        }
    }
}

NDFA::State NDFA::addInitialState() {
    State initial = addState();

    initialStates.add(initial);
    return initial;
}

void NDFA::makeInitialState(Automata::State state) {
    if (!isValid(state)) {
        throw std::logic_error(INVALID_STATE);
    }
    initialStates.add(state);
}

void NDFA::removeInitialState(NDFA::State state) {
    if (!isValid(state)) {
        throw std::logic_error(INVALID_STATE);
    }
    initialStates.remove(state);
}


NDFA::State NDFA::addFinalState() {
    State final = addState();

    finalStates.add(final);
    return final;
}

void NDFA::makeFinalState(NDFA::State state) {
    if (!isValid(state)) {
        throw std::logic_error(INVALID_STATE);
    }
    finalStates.add(state);
}

void NDFA::removeFinalState(NDFA::State state) {
    if (!isValid(state)) {
        throw std::logic_error(INVALID_STATE);
    }
    finalStates.remove(state);
}

void NDFA::addTransition(NDFA::State from, char with, NDFA::State to) {
//    if (!isValid(from) || !isValid(to)) {
//        throw std::logic_error(INVALID_STATE);
//    }
//
//    if (!alphabet.contains(with)) {
//        throw std::logic_error(INVALID_SYMBOL);
//    }

    auto &stateTr = transitions[from];
    auto tr = kstd::findIf(stateTr.begin(), stateTr.end(),
                                [with](const Transition &tr) {
                                    return tr.first == with;
                                });
    if (tr == stateTr.end()) {
        stateTr.pushBack({with, {{to}}});
    } else {
        tr->second.add(to);
    }
}

void NDFA::removeTransition(NDFA::State from, char with, NDFA::State to) {
    if (!isValid(from) || !isValid(to)) {
        throw std::logic_error(INVALID_STATE);
    }

    if (!alphabet.contains(with)) {
        throw std::logic_error(INVALID_SYMBOL);
    }

    auto &stateTr = transitions[from];
    auto tr = kstd::findIf(stateTr.begin(), stateTr.end(),
                                [with](const Transition &tr) {
                                    return tr.first == with;
                                });
    if (tr == stateTr.end()) {
        return;
    }
    tr->second.remove(to);
}

bool NDFA::isTotal() const {
    return kstd::allOf(transitions.begin(), transitions.end(),
                            [&](const Vector<Transition> &tr) {
                                return tr.size() == alphabet.size();
                            });
}

void NDFA::makeTotal() {
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

int NDFA::accepts(NDFA::State from, const char *word) const {
    if (*word == '\0') {
        return finalStates.contains(from);
    }

    auto &stateTr = transitions[from];
    char next = *word;

    auto tr = kstd::findIf(stateTr.begin(), stateTr.end(),
                                [next](const Transition &tr) {
                                    return tr.first == next;
                                });
    if (tr != stateTr.end()) {
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
    return 0;
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