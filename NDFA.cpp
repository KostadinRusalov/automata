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

    Algorithm::transform(initialStates.begin(), initialStates.end(),
                         initialStates.begin(), subtractOne);
    Algorithm::transform(finalStates.begin(), finalStates.end(),
                         finalStates.begin(), subtractOne);

    transitions.erase(transitions.cbegin() + state);

    for (auto &stateTr: transitions) {
        for (auto &tr: stateTr) {
            for (size_t i = 0; i < tr.second.size(); ++i) {
                auto &sts = tr.second;
                if (sts[i] == state) {
                    sts.erase(sts.cbegin() + i);
                    --i;
                } else if (sts[i] > state) {
                    --sts[i];
                }
            }
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
    if (!isValid(from) || !isValid(to)) {
        throw std::logic_error(INVALID_STATE);
    }

    if (!alphabet.contains(with)) {
        throw std::logic_error(INVALID_SYMBOL);
    }

    auto &stateTr = transitions[from];
    auto tr = Algorithm::findIf(stateTr.begin(), stateTr.end(),
                                [with](const Transition &tr) {
                                    return tr.first == with;
                                });
    if (tr == stateTr.end()) {
        stateTr.pushBack({with, {to}});
    } else {
        tr->second.pushBack(to);
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
    auto tr = Algorithm::findIf(stateTr.begin(), stateTr.end(),
                                [with](const Transition &tr) {
                                    return tr.first == with;
                                });
    if (tr == stateTr.end()) {
        return;
    }
    auto p = Algorithm::findIf(tr->second.cbegin(), tr->second.cend(),
                               [to](State state) {
                                   return state == to;
                               });
    tr->second.erase(p);
}

bool NDFA::isTotal() const {
    return Algorithm::allOf(transitions.begin(), transitions.end(),
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

    auto tr = Algorithm::findIf(stateTr.begin(), stateTr.end(),
                                [next](const Transition &tr) {
                                    return tr.first == next;
                                });
    if (tr != stateTr.end()) {
        int sum = 0;
        ++word;
        for (State state: tr->second) {
            sum += accepts(state, word);
        }
        return sum;
    }
    return 0;
}

bool NDFA::accepts(const char *word) const {
    int sum = 0;
    for (State initial: initialStates) {
        sum += accepts(initial, word);
    }
    return sum >= 1;
}


