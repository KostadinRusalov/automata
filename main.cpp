#include <iostream>
#include "cli/Command.h"
#include "cli/UI.h"
#include "cli/CommandFactory.h"
#include "tests/test.h"

int main() {
    UI::showCommands();
    auto factory = CommandFactory::instance();

    Command *command = factory.create();
    while (command) {
        command->run();
        delete command;
        command = factory.create();
    }

    return 0;
}