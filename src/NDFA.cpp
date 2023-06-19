#include "../include/NDFA.h"
#include "../include/helpers/SubtractOneAfter.hpp"
#include "../MyStructures/Queue/Queue.hpp"
#include "../include/CDFA.hpp"
#include "fstream"
#include "../include/helpers/File.hpp"

const char INVALID_STATE[] = "There is no such state in the DFA!";
const char INVALID_SYMBOL[] = "There is no such symbol in the alphabet_!";

NDFA::NDFA(const Automata::Alphabet &alphabet) {
    alphabet_ = alphabet;
}

NDFA::NDFA(Automata::Alphabet &&alphabet) {
    alphabet_ = std::move(alphabet);
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

void NDFA::validate(char s) const {
    if (!alphabet_.contains(s)) {
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
                return tr.size() == alphabet_.size();
            }
    );
}

void NDFA::makeTotal() {
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

void NDFA::findReachableStates(State from, BitSubset &reachable) const {
    for (auto &tr: transitions[from]) {
        for (auto s: tr.second) {
            if (!reachable.contains(s)) {
                reachable.add(s);
                findReachableStates(s, reachable);
            }
        }
    }
}

BitSubset NDFA::unreachableStates() const {
    BitSubset reachable(transitions.size());
    reachable.add(initialStates);
    for (auto initial: initialStates) {
        findReachableStates(initial, reachable);
    }
    return ~reachable;
}

void NDFA::removeUnreachableStates() {
    BitSubset unreachable = unreachableStates();
    for (int s = (int) lastState(); s >= 0; --s) {
        if (unreachable.contains(s)) {
            removeState(s);
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
    addSymbols(other.alphabet_);
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
        auto copy = finalStates;
        for (auto final: copy) {
            finalStates.remove(final);
        }
    }

    for (auto final: other.finalStates) {
        makeFinalState(offset(final, offsetIdx));
    }

    removeUnreachableStates();
    return *this;
}

NDFA NDFA::operator*() const {
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

CState<BitSubset> NDFA::createNewSubsetState(const CState<BitSubset> &from,
                                             char with, bool &isFinal) const {
    CState<BitSubset> newState;
    for (State s = 0; s < transitions.size(); ++s) {
        if (!from.data().contains(s)) { continue; }

        auto tr = findTransition(transitions[s], with);
        if (tr != transitions[s].end()) {
            newState.data().add(tr->second);

            if (tr->second.intersectsWith(finalStates)) {
                isFinal = true;
            }
        }
    }
    return newState;
}

NDFA::State NDFA::addNewState(CDFA<BitSubset> &d, NDFA::State from, char with,
                              CState<BitSubset> &&newState, bool isFinal) {
    auto to = d.findState(newState);

    if (to != d.errorState()) {
        d.addTransition(from, with, to);
        return d.errorState();
    }

    auto nState = d.addState(std::move(newState));
    d.addTransition(from, with, nState);

    if (isFinal) {
        d.makeFinalState(nState);
    }

    return nState;
}

DFA NDFA::determinized() const {
    CDFA<BitSubset> d(alphabet_);

    auto init = d.addInitialState({initialStates.elements()});
    if (initialStates.intersectsWith(finalStates)) {
        d.makeFinalState(init);
    }
    Queue<State> created;
    created.push(init);

    while (!created.empty()) {
        auto state = created.peek();
        auto cState = *d.findCState(state);
        created.pop();

        for (char s: alphabet_) {
            bool isFinal = false;
            auto newState = createNewSubsetState(cState, s, isFinal);

            if (!newState.data().empty()) {
                auto next = addNewState(d, state, s, std::move(newState), isFinal);
                if (next != d.errorState()) {
                    created.push(next);
                }
            }
        }
    }

    return d;
}

NDFA NDFA::reversed() const {
    NDFA rev(alphabet_);

    BitSubset initials(finalStates.elements());
    BitSubset finals(finalStates.elements());

    return rev;
}

DFA NDFA::minimized() const {
    return determinized().reversed().determinized().reversed().determinized();
}

bool NDFA::isEmptyLanguage() const {
    if (initialStates.empty() || finalStates.empty()) {
        return true;
    }

    auto unreachable = unreachableStates();
    return kstd::allOf(finalStates.begin(), finalStates.end(),
                       [unreachable](State s) { return unreachable.contains(s); });
}

void NDFA::saveTo(std::ofstream &binaryFile) const {
    saveAlphabetTo(binaryFile);

    size_t statesCount = transitions.size();
    binaryFile.write((const char *) &statesCount, sizeof(size_t));

    for (auto &stateTr: transitions) {
        size_t trCount = stateTr.size();
        binaryFile.write((const char *) &trCount, sizeof(size_t));

        for (auto &tr: stateTr) {
            char with = tr.first;
            binaryFile.write((const char *) &with, sizeof(char));
            File::saveSet(binaryFile, tr.second);
        }
    }

    File::saveSet(binaryFile, initialStates);
    File::saveSet(binaryFile, finalStates);
}

NDFA NDFA::readFrom(std::ifstream &binaryFile) {
    NDFA n;
    n.readAlphabetFrom(binaryFile);

    size_t statesCount;
    binaryFile.read((char *) &statesCount, sizeof(size_t));
    for (size_t s = 0; s < statesCount; ++s) {
        n.addState();
        size_t trCount;
        binaryFile.read((char *) &trCount, sizeof(size_t));

        for (size_t t = 0; t < trCount; ++t) {
            n.transitions[s].pushBack({});
            auto &tr = n.transitions[s][t];
            binaryFile.read((char *) &tr.first, sizeof(char));
            File::readSet(binaryFile, tr.second);
        }
    }

    File::readSet(binaryFile, n.initialStates);
    File::readSet(binaryFile, n.finalStates);
    return n;
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

void print(const NDFA &ndfa) {
    using File::print;
    print(ndfa.alphabet());

    std::cout << "initial states: ";
    print(ndfa.initialStates);

    std::cout << "final states: ";
    print(ndfa.finalStates);

    for (size_t s = 0; s < ndfa.transitions.size(); ++s) {
        std::cout << "q_" << s << ":";
        for (auto &tr: ndfa.transitions[s]) {
            std::cout << " " << tr.first << " -> { ";
            for (auto st: tr.second) {
                std::cout << st << ' ';
            }
            std::cout << "};";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}