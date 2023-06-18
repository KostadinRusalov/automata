#include "../include/GNFA.h"

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

void GNFA::addTransition(Automata::State from, const Regex &with, Automata::State to) {
    if (from == finalState || to == initialState) {
        throw std::invalid_argument("cannot make such transition!");
    }

    auto &stateTr = transitions[from];

    auto tr = transitions[from].begin() + (findTransition(from, to) - transitions[from].cend());

    if (tr == stateTr.end()) {
        stateTr.pushBack({with, to});
    } else {
        tr->first = Regex(new Regex::Union(tr->first.expr->clone(), with.expr->clone()));
    }
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

    gnfa.addTransition(gnfa.initialState, Regex(new Regex::EmptyWord), shift(dfa.initialState));

    for (State s = 0; s < dfa.transitions.size(); ++s) {
        for (auto &tr: dfa.transitions[s]) {
            gnfa.addTransition(shift(s), Regex(new Regex::Letter(tr.first)),
                               shift(tr.second));
        }
    }

    for (State final: dfa.finalStates) {
        gnfa.addTransition(shift(final), Regex(new Regex::EmptyWord), gnfa.finalState);
    }
    return gnfa;
}

Regex GNFA::convertToRegex() const {
    if (transitions.size() == 2) {
        return transitions[initialState].front().first;
    }

    GNFA g;
    g.setAlphabet(alphabet());

    g.transitions.resize(transitions.size() - 1);

    State rip = finalState - 1;
    for (auto i = initialState; i < finalState; ++i) {
        for (auto j = initialState + 1; j <= finalState; ++j) {
            auto r1 = findRegex(i, rip);
            auto r2 = findRegex(rip, rip);
            auto r3 = findRegex(rip, j);
            auto r4 = findRegex(i, j);

            auto r = Regex(
                    new Union(
                            new Concat(r1.expr->clone(),
                                       new Concat(new KleeneStar(r2.expr->clone()),
                                                  r3.expr->clone())),
                            r4.expr->clone()
                    )
            );
            g.addTransition(i, r, j);
        }
    }

    return g.convertToRegex();
}

GNFA::transitionIterator GNFA::findTransition(Automata::State from, Automata::State to) const {
    return kstd::findIf(transitions[from].begin(), transitions[from].end(),
                        [to](const Transition &tr) { return tr.second == to; });
}

Regex GNFA::findRegex(Automata::State from, Automata::State to) const {
    auto tr = findTransition(from, to);
    return tr == transitions[from].end()
           ? Regex(new Regex::EmptyLanguage) : Regex(tr->first);
}

