#include "NDFA.h"
#include "SubtractOneAfter.hpp"
#include "MyStructures/Queue/Queue.hpp"

const char INVALID_STATE[] = "There is no such state in the DFA!";
const char INVALID_SYMBOL[] = "There is no such symbol in the alphabet!";

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

void NDFA::validate(char s) const {
    if (!alphabet.contains(s)) {
        throw std::logic_error(INVALID_STATE);
    }
}

void NDFA::validate(State state) const {
    if (state >= transitions.size()) {
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
    return kstd::findIf(
            stateTr.begin(), stateTr.end(),
            [with](const Transition &tr) {
                return tr.first == with;
            }
    );
}

void NDFA::addTransition(NDFA::State from, char with, NDFA::State to) {
    validate(from);
    // doesn't validate the symbol or the destination
    // for the algorithms
    auto &stateTr = transitions[from];
    auto tr = stateTr.begin() + (findTransition(stateTr, with) - stateTr.cbegin());

    if (tr == stateTr.end()) {
        stateTr.pushBack({with, {to}});
    } else {
        tr->second.add(to);
    }
}

void NDFA::removeTransition(NDFA::State from, char with, NDFA::State to) {
    validate(from);

    auto &stateTr = transitions[from];
    auto constTr = findTransition(stateTr, with);
    auto tr = stateTr.begin() + (constTr - stateTr.cbegin());

    if (tr != stateTr.end()) {
        if (tr->second.size() >= 2) {
            tr->second.remove(to);
        } else {
            stateTr.erase(constTr);
        }
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
    for (State q = 0; q < transitions.size(); ++q) {
        auto &stateTr = transitions[q];
        if (stateTr.size() == alphabet.size()) {
            continue;
        }

        Alphabet leftSymbols(alphabet);
        for (auto &tr: stateTr) {
            leftSymbols.remove(tr.first);
        }

        for (char symbol: leftSymbols) {
            addTransition(q, symbol, dump);
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

    int successPaths = 0;
    ++word;
    for (State state: tr->second) {
        successPaths += accepts(state, word);

        if (successPaths >= 1) {
            return successPaths;
        }
    }
    return successPaths;
}

BitSubset NDFA::unreachableStates() const {
    BitSubset reachable(transitions.size());

    reachable.add(initialStates);
    for (auto &stateTr: transitions) {
        for (auto &tr: stateTr) {
            reachable.add(tr.second);
        }
    }
    return ~reachable;
}

void NDFA::removeUnreachableStates() {
    BitSubset unreachable = unreachableStates();
    for (State s = 0; s < transitions.size(); ++s) {
        if (unreachable.contains(s)) {
            removeState(s);
        }
    }
}

bool NDFA::accepts(const char *word) const {
    int successPaths = 0;
    for (State initial: initialStates) {
        successPaths += accepts(initial, word);

        if (successPaths >= 1) {
            return true;
        }
    }
    return false;
}

NDFA::State NDFA::offset(NDFA::State state, NDFA::State offsetIdx) {
    return state + offsetIdx;
}

void NDFA::copyTransitions(NDFA::State state, const Vector<NDFA::Transition> &stateTr, NDFA::State offsetIdx) {
    for (auto &tr: stateTr) {
        for (auto st: tr.second) {
            addTransition(state, tr.first, offset(st, offsetIdx));
        }
    }
}

NDFA &NDFA::operator+=(const NDFA &other) {
    addSymbols(other.alphabet);
    size_t offsetIdx = transitions.size();

    for (auto &stateTr: other.transitions) {
        copyTransitions(addState(), stateTr, offsetIdx);
    }

    return *this;
}

NDFA &NDFA::operator|=(const NDFA &other) {
    size_t offsetIdx = transitions.size();
    operator+=(other);

    for (auto initial: other.initialStates) {
        makeInitialState(offset(initial, offsetIdx));
    }

    for (auto final: other.finalStates) {
        makeFinalState(offset(final, offsetIdx));
    }
    return *this;
}

NDFA &NDFA::operator*=(const NDFA &other) {
    size_t offsetIdx = transitions.size();
    operator+=(other);

    for (auto final: finalStates) {
        for (auto initial: other.initialStates) {
            copyTransitions(final, other.transitions[initial], offsetIdx);
        }
    }

    if (!other.initialStates.intersectsWith(other.finalStates)) {
        for (auto final: finalStates) {
            finalStates.remove(final);
        }
    }

    for (auto final: other.finalStates) {
        makeFinalState(offset(final, offsetIdx));
    }

    removeUnreachableStates();
    return *this;
}

NDFA NDFA::operator*() {
    NDFA n(*this);

    n.initialStates.clear();
    n.makeFinalState(n.addInitialState());

    for (auto final: n.finalStates) {
        for (auto initial: initialStates) {
            n.copyTransitions(final, transitions[initial], 0);
        }
    }

    n.removeUnreachableStates();
    return n;
}

namespace {
    void f() {

    }
}

DFA NDFA::getDeterminized() const {
    CDFA<BitSubset> d(alphabet);

    Queue<State> created;
    created.push(d.addInitialState({initialStates.elements()}));

    while (!created.empty()) {
        auto qState = created.peek();
        auto q = *d.findCState(qState);
        created.pop();

        for (char s: alphabet) {
            CState<BitSubset> newState;
            bool isFinal = false;

            for (State st = 0; st < transitions.size(); ++st) {
                if (!q.data().contains(st)) { continue; }

                auto tr = findTransition(transitions[st], s);
                if (tr != transitions[st].end()) {
                    newState.data().add(tr->second);

                    if (tr->second.intersectsWith(finalStates)) {
                        isFinal = true;
                    }
                }
            }
            if (newState.data().empty()) { continue; }

            auto state = d.findState(newState);
            if (state != d.errorState()) {
                d.addTransition(qState, s, state);
                continue;
            }

            auto nState = d.addState(std::move(newState));
            d.addTransition(qState, s, nState);
            created.push(nState);

            if (isFinal) { d.makeFinalState(nState); }
        }
    }

    return d;
}


NDFA operator+(const NDFA &rhs, const NDFA &lhs) {
    NDFA n(rhs);
    n += lhs;
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