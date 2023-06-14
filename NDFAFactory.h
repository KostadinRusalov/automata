#pragma once

#include "NDFA.h"

class NDFAFactory {
public:
    static NDFA exact(const NDFA::Alphabet &alphabet, const char *word) {
        NDFA n;
        n.setAlphabet(alphabet);

        auto q = n.addInitialState();

        while (*word) {
            auto next = n.addState();
            n.addTransition(q, *word, next);

            q = next;
            ++word;
        }

        n.makeFinalState(q);
        return n;
    }

    static NDFA prefix(const NDFA::Alphabet &alphabet, const char *word) {
        NDFA n;
        n.setAlphabet(alphabet);

        auto q = n.addInitialState();

        while (*word) {
            auto next = n.addState();
            n.addTransition(q, *word, next);

            q = next;
            ++word;
        }

        n.makeFinalState(q);

        for (char c: alphabet) {
            n.addTransition(q, c, q);
        }

        return n;
    }

    static NDFA suffix(const NDFA::Alphabet &alphabet, const char *word) {
        NDFA n;
        n.setAlphabet(alphabet);

        auto q = n.addInitialState();

        for (char c: alphabet) {
            n.addTransition(q, c, q);
        }

        while (*word) {
            auto next = n.addState();
            n.addTransition(q, *word, next);

            q = next;
            ++word;
        }

        n.makeFinalState(q);

        return n;
    }

    static NDFA infix(const NDFA::Alphabet &alphabet, const char *word) {
        NDFA n;
        n.setAlphabet(alphabet);

        auto q = n.addInitialState();

        for (char c: alphabet) {
            n.addTransition(q, c, q);
        }

        while (*word) {
            auto next = n.addState();
            n.addTransition(q, *word, next);

            q = next;
            ++word;
        }

        n.makeFinalState(q);
        for (char c: alphabet) {
            n.addTransition(q, c, q);
        }
        return n;
    }


};

