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
        case 2:
            return determinize();
        case 3:
            return totalize();
        case 4:
            return accepts();
        case 5:
            return isEmptyLanguage();
        case 6:
            return unionOf();
        case 7:
            return concat();
        case 8:
            return kleeneStar();
        case 9:
            return help();
        case 10:
        default:
            return nullptr;
    }
}

Command *CommandFactory::createNDFA() {
    std::cout << "enter regex: ";
    String buff;
    std::cin >> buff;

    return new CreateNDFA(buff);
}

Command *CommandFactory::totalize() {
    std::cout << "idx: ";
    size_t idx;
    std::cin >> idx;
    return new Totalize(idx);
}

Command *CommandFactory::determinize() {
    std::cout << "idx: ";
    size_t idx;
    std::cin >> idx;
    return new Determinize(idx);
}

Command *CommandFactory::accepts() {
    std::cout << "idx: ";
    size_t idx;
    std::cin >> idx;
    std::cout << std::endl;

    std::cout << "word: ";
    String word;
    std::cin >> word;
    return new Accepts(idx, word);
}

Command *CommandFactory::isEmptyLanguage() {
    std::cout << "idx: ";
    size_t idx;
    std::cin >> idx;
    return new IsEmptyLanguage(idx);
}

Command *CommandFactory::unionOf() {
    std::cout << "rhs: ";
    size_t rhs;
    std::cin >> rhs;
    std::cout << std::endl;
    std::cout << "lhs: ";
    size_t lhs;
    std::cin >> lhs;
    return new Union(rhs, lhs);
}

Command *CommandFactory::concat() {
    std::cout << "rhs: ";
    size_t rhs;
    std::cin >> rhs;
    std::cout << std::endl;
    std::cout << "lhs: ";
    size_t lhs;
    std::cin >> lhs;
    return new Concat(rhs, lhs);
}

Command *CommandFactory::kleeneStar() {
    std::cout << "idx: ";
    size_t idx;
    std::cin >> idx;
    return new KleeneStar(idx);
}

Command *CommandFactory::help() {
    return new Help();
}
