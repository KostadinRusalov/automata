#include "CommandFactory.h"
#include "UI.h"

CommandFactory &CommandFactory::instance() {
    static CommandFactory instance_;
    return instance_;
}

Command *CommandFactory::create() const {
    short commandIdx;
    std::cin >> commandIdx;

    switch (commandIdx) {
        case 1:
            return createNDFA();
//        case 2:
//            return determinize();
//        case 3:
//            return totalize();
//        case 4:
//            return accepts();
//        case 5:
//            return isEmptyLanguage();
//        case 6:
//            return unionOf();
//        case 7:
//            return concat();
//        case 8:
//            return kleeneStar();
//        case 9:
//            return help();
//        default:
//            return invalidCommand();
    }
}

Command *CommandFactory::createNDFA() {
    std::cout << "enter regex: ";
    String buff;
    std::cin >> buff;

    return new CreateNDFA(buff);
}