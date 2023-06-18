#pragma once

#include "Command.h"

class CommandFactory {
public:
    static CommandFactory &instance();

    Command *create() const;

private:
    CommandFactory() = default;

    static Command *createNDFA();

    static Command *totalize();

    static Command *determinize();

    static Command *accepts();

    static Command *isEmptyLanguage();

    static Command *unionOf();

    static Command *concat();

    static Command *kleeneStar();

    static Command *help();
};