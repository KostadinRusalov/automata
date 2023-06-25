#include "../include/GNFA.h"
#include <cstring>

typedef Regex::Letter Letter;
typedef Regex::EmptyWord EmptyWord;
typedef Regex::EmptyLanguage EmptyLanguage;
typedef Regex::Union Union;
typedef Regex::Concat Concat;
typedef Regex::KleeneStar KleeneStar;

Automata::State GNFA::addState() {
    transitions.pushBack({});
    return lastState();
}

void GNFA::removeState(Automata::State state) {
    if (state <= initialState && state >= finalState) {
        return;
    }

    transitions.erase(transitions.cbegin() + state);
    for (auto &stateTr: transitions) {
        for (int to = 0; to < stateTr.size(); ++to) {
            if (stateTr[to].first == state) {
                stateTr.erase(stateTr.cbegin() + to);
                --to;
            } else if (stateTr[to].first > state) {
                --stateTr[to].first;
            }
        }
    }

    --finalState;
}

Automata::State GNFA::lastState() const {
    return transitions.size() - 1;
}

void GNFA::addInitialState() {
    transitions.pushBack({});
    initialState = 0;
}

void GNFA::addFinalState() {
    transitions.pushBack({});
    finalState = lastState();
}

void GNFA::addTransition(Automata::State from, Automata::State to, const String &regex) {
    auto &stateTr = transitions[from];
    auto tr = stateTr.begin() + (findTransition(from, to) - stateTr.cbegin());

    if (tr == stateTr.end()) {
        stateTr.pushBack({to, regex});
    } else {
        tr->second += "+" + regex;
    }
}

GNFA::TempTransitions GNFA::findRightStates(Automata::State state) const {
    TempTransitions right;

    for (State s = 0; s < transitions.size(); ++s) {
        if (s == state) {
            continue;
        }
        auto tr = findTransition(s, state);
        if (tr != transitions[s].cend()) {
            right.pushBack({s, tr->second});
        }
    }
    return right;

}

GNFA::TempTransitions GNFA::findLeftStates(Automata::State state) const {
    TempTransitions left;

    for (State s = 0; s < transitions.size(); ++s) {
        if (s == state) {
            continue;
        }
        auto tr = findTransition(state, s);
        if (tr != transitions[state].cend()) {
            left.pushBack({s, tr->second});
        }
    }
    return left;

}

String GNFA::mergedRegexes(const StringView &right, const StringView &left, const String &loop) {
    String merged;
    bool notEmpty = false;
    if (strcmp(right.data(), Regex::EPSILON.c_str()) != 0) {
        merged += right;
        notEmpty = true;
    }

    if (loop.size() > 0) {
        merged += "(" + loop + ")*";
        notEmpty = true;
    }

    if (strcmp(left.data(), Regex::EPSILON.c_str()) != 0) {
        merged += left;
        notEmpty = true;
    }

    return notEmpty ? merged : Regex::EPSILON;
}

void GNFA::ripState(Automata::State rip) {
    auto rightStates = findRightStates(rip);
    auto leftStates = findLeftStates(rip);

    String loop;
    auto loopTr = findTransition(rip, rip);

    if (loopTr != transitions[rip].cend()) {
        loop += loopTr->second;
    }

    for (auto r: rightStates) {
        for (auto l: leftStates) {
            addTransition(r.first, l.first, mergedRegexes(r.second, l.second, loop));
        }
    }

    removeState(rip);
}

GNFA GNFA::convertFrom(const DFA &dfa) {
    GNFA gnfa;
    gnfa.setAlphabet(dfa.alphabet());

    gnfa.addInitialState();
    for (State s = 0; s < dfa.transitions.size(); ++s) {
        gnfa.addState();
    }
    gnfa.addFinalState();

    auto shift = [](State s) { return s + 1; };

    gnfa.addTransition(gnfa.initialState, shift(dfa.initialState), Regex::EPSILON);

    for (State s = 0; s < dfa.transitions.size(); ++s) {
        for (auto &tr: dfa.transitions[s]) {
            String r;
            r += tr.first;
            gnfa.addTransition(shift(s), shift(tr.second), r);
        }
    }

    for (State final: dfa.finalStates) {
        gnfa.addTransition(shift(final), gnfa.finalState, Regex::EPSILON);
    }
    return gnfa;
}

String GNFA::convertToRegex() const {
    GNFA copy(*this);

    while (copy.transitions.size() > 2) {
        copy.ripState(copy.transitions.size() - 2);
    }

    return copy.transitions[initialState].front().second;
}

GNFA::transitionIterator GNFA::findTransition(Automata::State from, Automata::State to) const {
    return kstd::findIf(transitions[from].begin(), transitions[from].end(),
                        [to](const Transition &tr) { return tr.first == to; });
}

