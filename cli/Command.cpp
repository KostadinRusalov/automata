#include "Command.h"
#include "UI.h"

CreateNDFA::CreateNDFA(const StringView &regex) : regex(regex) {}

void Command::validate(size_t idx) const {
    if (idx >= automatas.size()) {
        UI::invalidIndex();
    }
}

void CreateNDFA::run() {
    automatas.pushBack(regex.toNDFA());
    print(automatas.back());
}

Determinize::Determinize(size_t idx) : idx(idx) {}

void Determinize::run() {
    validate(idx);

    DFA d = automatas[idx].determinized();
    print(d);
}

Totalize::Totalize(size_t idx) : idx(idx) {}

void Totalize::run() {
    validate(idx);

    automatas[idx].makeTotal();
    print(automatas[idx]);
}

Accepts::Accepts(size_t idx, const String &word)
        : idx(idx), word(word) {}

void Accepts::run() {
    validate(idx);

    std::cout << automatas[idx].accepts(word.c_str());
}


IsEmptyLanguage::IsEmptyLanguage(size_t idx) : idx(idx) {}

void IsEmptyLanguage::run() {
    validate(idx);

    std::cout << automatas[idx].isEmptyLanguage();
}

Union::Union(size_t rhs, size_t lhs) : rhs(rhs), lhs(lhs) {}

void Union::run() {
    validate(rhs);
    validate(lhs);

    NDFA n = automatas[rhs] | automatas[lhs];
    automatas.pushBack(std::move(n));
    print(automatas.back());
}

Concat::Concat(size_t rhs, size_t lhs) : rhs(rhs), lhs(lhs) {}

void Concat::run() {
    validate(rhs);
    validate(lhs);

    NDFA n = automatas[rhs] * automatas[lhs];
    automatas.pushBack(std::move(n));
    print(automatas.back());
}

KleeneStar::KleeneStar(size_t idx) : idx(idx) {}

void KleeneStar::run() {
    validate(idx);

    NDFA n = *automatas[idx];
    automatas.pushBack(std::move(n));
    print(automatas.back());
}

void Help::run() {
    UI::showCommands();
}
