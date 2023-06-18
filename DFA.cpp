#include "DFA.h"
#include "NDFA.h"

#include "MyStructures/Algorithm.hpp"
#include "MyStructures/Queue/Queue.hpp"
#include "SubtractOneAfter.hpp"
#include <fstream>
#include "File.hpp"

const char INVALID_STATE[] = "There is no such state in the DFA!";
const char INVALID_SYMBOL[] = "There is no such symbol in the alphabet_!";

DFA::DFA(const Automata::Alphabet &alphabet) {
    alphabet_ = alphabet;
}

DFA::DFA(Automata::Alphabet &&alphabet) {
    alphabet_ = std::move(alphabet);
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

void DFA::validate(State state) const {
    if (state >= transitions.size()) {
        throw std::logic_error(INVALID_STATE);
    }
}

void DFA::validate(char s) const {
    if (!alphabet_.contains(s)) {
        throw std::logic_error(INVALID_STATE);
    }
}

void DFA::removeState(DFA::State state) {
    validate(state);

    if (state == initialState) {
        initialState = errorState();
    }

    removeAndShift(state, finalStates);

    transitions.erase(transitions.cbegin() + state);
    for (auto &stateTr: transitions) {
        for (size_t to = 0; to < stateTr.size(); ++to) {
            if (stateTr[to].second == state) {
                stateTr.erase(stateTr.cbegin() + to);
                --to;
            } else if (stateTr[to].second > state) {
                --stateTr[to].second;
            }
        }
    }
}

DFA::State DFA::addInitialState() {
    initialState = addState();
    return initialState;
}

void DFA::makeInitialState(DFA::State state) {
    validate(state);
    initialState = state;
}


DFA::State DFA::addFinalState() {
    State final = addState();
    finalStates.add(final);
    return final;
}

void DFA::makeFinalState(DFA::State state) {
    validate(state);
    finalStates.add(state);
}

void DFA::removeFinalState(DFA::State state) {
    validate(state);
    finalStates.remove(state);
}

void DFA::addTransition(DFA::State from, char with, DFA::State to) {
    validate(from);
    validate(to);
    validate(with);

    auto &stateTr = transitions[from];
    auto tr = kstd::findIf(
            stateTr.begin(), stateTr.end(),
            [with](const Transition &tr) {
                return tr.first == with;
            }
    );
    if (tr == stateTr.end()) {
        stateTr.pushBack({with, to});
    } else {
        tr->second = to;
    }
}

void DFA::removeTransition(DFA::State from, char with, DFA::State to) {
    validate(from);
    validate(to);
    validate(with);

    if (!alphabet_.contains(with)) {
        throw std::logic_error(INVALID_SYMBOL);
    }

    auto &st = transitions[from];
    Transition transition(with, to);
    auto pos = kstd::find(st.cbegin(), st.cend(), transition);

    if (pos != st.cend()) {
        st.erase(pos);
    }
}

bool DFA::isTotal() const {
    return kstd::allOf(
            transitions.begin(), transitions.end(),
            [&](const Vector<Transition> &tr) {
                return tr.size() == alphabet_.size();
            }
    );
}

void DFA::makeTotal() {
    if (isTotal()) { return; }

    State deadState = addState();
    for (State q = 0; q < transitions.size(); ++q) {
        auto &stateTr = transitions[q];
        if (stateTr.size() == alphabet_.size()) {
            continue;
        }

        Alphabet leftSymbols(alphabet_);
        for (auto &tr: stateTr) {
            leftSymbols.remove(tr.first);
        }

        for (char symbol: leftSymbols) {
            addTransition(q, symbol, deadState);
        }
    }
}

DFA::State DFA::nextState(DFA::State current, char s) const {
    for (auto &tr: transitions[current]) {
        if (tr.first == s) {
            return tr.second;
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

    return next != errorState() &&
           finalStates.contains(next);
}

NDFA DFA::reversed() const {
    NDFA rev(alphabet_);

    for (State s = 0; s <= lastState(); ++s) {
        rev.addState();
    }

    BitSubset finals(finalStates.elements());
    for (State s = 0; s <= lastState(); ++s) {
        for (auto &tr: transitions[s]) {
            rev.addTransition(tr.second, tr.first, s);
        }
        if (finals.contains(s)) {
            rev.makeInitialState(s);
        }
        if (s == initialState) {
            rev.makeFinalState(s);
        }
    }

    return rev;
}

void DFA::findReachableStates(Automata::State from, BitSubset &reachable) const {
    for (auto &tr: transitions[from]) {
        if (!reachable.contains(tr.second)) {
            reachable.add(tr.second);
            findReachableStates(tr.second, reachable);
        }
    }
}

BitSubset DFA::unreachableStates() const {
    BitSubset reachable(transitions.size());
    reachable.add(initialState);
    findReachableStates(initialState, reachable);
    return ~reachable;
}

void DFA::removeUnreachableStates() {
    BitSubset unreachable = unreachableStates();
    for (int s = (int) lastState(); s >= 0; --s) {
        if (unreachable.contains(s)) {
            removeState(s);
        }
    }
}

DFA DFA::minimized() const {
    return reversed().determinized().reversed().determinized();
}

bool DFA::isEmptyLanguage() const {
    if (initialState == errorState() || finalStates.empty()) {
        return true;
    }

    auto unreachable = unreachableStates();
    return kstd::allOf(finalStates.begin(), finalStates.end(),
                       [unreachable](State s) { return unreachable.contains(s); });
}

DFA DFA::operator!() const {
    DFA complement(*this);
    complement.makeTotal();

    complement.finalStates.clear();
    BitSubset finals(finalStates.elements());

    for (State s = 0; s <= complement.lastState(); ++s) {
        if (!finals.contains(s)) {
            complement.makeFinalState(s);
        }
    }
    return complement;
}

DFA operator|(const DFA &rhs, const DFA &lhs) {
    return !(!rhs & !lhs);
}

DFA operator/(const DFA &rhs, const DFA &lhs) {
    return rhs & !lhs;
}

DFA operator&(const DFA &rhs, const DFA &lhs) {
    // TODO
}

DFA DFA::total() const {
    DFA t(*this);
    t.makeTotal();
    return t;
}

Vector<DFA::Transition>::const_iterator DFA::findTransition(const Vector<Transition> &stateTr, char with) {
    return kstd::findIf(
            stateTr.begin(), stateTr.end(),
            [with](const Transition &tr) {
                return tr.first == with;
            }
    );
}


void DFA::saveTo(std::ofstream &binaryFile) const {
    saveAlphabetTo(binaryFile);

    size_t statesCount = transitions.size();
    binaryFile.write((const char *) &statesCount, sizeof(size_t));

    for (auto &stateTr: transitions) {
        File::saveVector(binaryFile, stateTr);
    }

    binaryFile.write((const char *) &initialState, sizeof(State));
    File::saveSet(binaryFile, finalStates);
}

DFA DFA::readFrom(std::ifstream &binaryFile) {
    DFA a;
    a.readAlphabetFrom(binaryFile);

    size_t statesCount;
    binaryFile.read((char *) &statesCount, sizeof(size_t));

    for (size_t s = 0; s < statesCount; ++s) {
        a.addState();
        File::readVector(binaryFile, a.transitions[s]);
    }

    binaryFile.read((char *) &a.initialState, sizeof(State));
    File::readSet(binaryFile, a.finalStates);

    return a;
}
