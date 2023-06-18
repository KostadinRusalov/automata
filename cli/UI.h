#pragma once

#include <iostream>
#include "../include/NDFA.h"
#include "../include/DFA.h"

namespace UI {
    const short LINE_LEN = 128;

    void clearConsole();

    void showCommands();

    void invalidIndex();
}