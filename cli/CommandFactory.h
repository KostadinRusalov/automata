#pragma once

#include "Command.h"

class CommandFactory {
public:
    static CommandFactory &instance();

    Command *create() const;

private:
    CommandFactory() = default;

    static Command *createNDFA() ;
};