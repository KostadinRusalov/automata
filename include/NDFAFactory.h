#pragma once

#include "NDFA.h"
#include "../MyStructures/StringView/StringView.h"

namespace NDFAFactory {
    NDFA exact(const NDFA::Alphabet &alphabet, const char *word);

    NDFA exact(const char *word);

    NDFA exact(char letter);

    NDFA exact(const StringView &word);

    NDFA prefix(const NDFA::Alphabet &alphabet, const char *word);

    NDFA prefix(const char *word);

    NDFA suffix(const NDFA::Alphabet &alphabet, const char *word);

    NDFA suffix(const char *word);

    NDFA infix(const NDFA::Alphabet &alphabet, const char *word);

    NDFA infix(const char *word);

    NDFA emptyLanguage();

    NDFA emptyWord();
};