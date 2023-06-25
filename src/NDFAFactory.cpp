#include "../include/NDFAFactory.h"

namespace {
    void addConsecutiveTransitions(NDFA &n, NDFA::State &from, const char *word, bool alphabet = false) {
        while (*word) {
            if (alphabet) {
                n.addSymbol(*word);
            }

            auto next = n.addState();
            n.addTransition(from, *word, next);

            from = next;
            ++word;
        }
    }

    void addSelfTransitions(NDFA &n, NDFA::State q) {
        for (char c: n.alphabet()) {
            n.addTransition(q, c, q);
        }
    }
}

NDFA NDFAFactory::exact(const Automata::Alphabet &alphabet, const char *word) {
    NDFA n(alphabet);

    auto q = n.addInitialState();

    addConsecutiveTransitions(n, q, word);

    n.makeFinalState(q);
    return n;
}

NDFA NDFAFactory::exact(const char *word) {
    NDFA n;
    auto q = n.addInitialState();
    addConsecutiveTransitions(n, q, word, true);

    n.makeFinalState(q);
    return n;
}

NDFA NDFAFactory::exact(char letter) {
    NDFA n;
    n.addSymbol(letter);

    auto q = n.addInitialState();
    auto f = n.addFinalState();
    n.addTransition(q, letter, f);

    return n;
}

NDFA NDFAFactory::exact(const StringView &word) {
    if (word.empty()) {
        return NDFAFactory::emptyWord();
    }

    NDFA n;
    auto q = n.addInitialState();
    for (char s: word) {
        n.addSymbol(s);
        auto next = n.addState();
        n.addTransition(q, s, next);
        q = next;
    }

    n.makeFinalState(q);
    return n;
}

NDFA NDFAFactory::prefix(const Automata::Alphabet &alphabet, const char *word) {
    NDFA n(alphabet);

    auto q = n.addInitialState();

    addConsecutiveTransitions(n, q, word);

    n.makeFinalState(q);

    addSelfTransitions(n, q);
    return n;
}

NDFA NDFAFactory::prefix(const char *word) {
    NDFA n;

    auto q = n.addInitialState();

    addConsecutiveTransitions(n, q, word, true);

    n.makeFinalState(q);

    addSelfTransitions(n, q);
    return n;
}

NDFA NDFAFactory::suffix(const Automata::Alphabet &alphabet, const char *word) {
    NDFA n(alphabet);

    auto q = n.addInitialState();

    addSelfTransitions(n, q);

    addConsecutiveTransitions(n, q, word);

    n.makeFinalState(q);

    return n;
}

NDFA NDFAFactory::suffix(const char *word) {
    NDFA n;

    auto q = n.addInitialState();

    auto initial = q;
    addConsecutiveTransitions(n, q, word, true);

    n.makeFinalState(q);
    addSelfTransitions(n, initial);

    return n;
}

NDFA NDFAFactory::infix(const NDFA::Alphabet &alphabet, const char *word) {
    NDFA n(alphabet);

    auto q = n.addInitialState();

    addSelfTransitions(n, q);

    addConsecutiveTransitions(n, q, word);

    n.makeFinalState(q);

    addSelfTransitions(n, q);
    return n;

}

NDFA NDFAFactory::infix(const char *word) {
    NDFA n;

    auto q = n.addInitialState();

    addSelfTransitions(n, q);

    addConsecutiveTransitions(n, q, word, true);

    n.makeFinalState(q);

    addSelfTransitions(n, q);
    return n;
}

NDFA NDFAFactory::emptyLanguage() {
    NDFA n;
    n.addInitialState();
    return n;
}

NDFA NDFAFactory::emptyWord() {
    NDFA n;
    n.makeFinalState(n.addInitialState());
    return n;
}