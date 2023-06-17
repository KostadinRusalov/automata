#include "Regex.h"
#include "NDFAFactory.h"

Regex::Regex() {
    expression = new EmptyLanguage();
}

Regex::Regex(const String &regex) {
    expression = parse(regex);
}

Regex::Regex(const Regex &other) {
    copyFrom(other);
}

Regex::Regex(Regex &&other) noexcept {
    moveFrom(std::move(other));
}

Regex &Regex::operator=(const Regex &other) {
    if (this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

Regex &Regex::operator=(Regex &&other) noexcept {
    if (this != &other) {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

Regex::~Regex() {
    free();
}

void Regex::free() {
    delete expression;
}

void Regex::copyFrom(const Regex &other) {
    expression = other.expression->clone();
}

void Regex::moveFrom(Regex &&other) {
    expression = other.expression;
    other.expression = nullptr;
}

Regex::Expression *Regex::parse(const String &expression) {
    return nullptr;
}

Regex::Binary::Binary(Expression *rhs, Expression *lhs)
        : rhs(rhs), lhs(lhs) {}

Regex::Binary::~Binary() {
    delete rhs;
    delete lhs;
}


Regex::Union::Union(Regex::Expression *rhs, Regex::Expression *lhs)
        : Binary(rhs, lhs) {}

NDFA Regex::Union::createNDFA() {
    return rhs->createNDFA() | lhs->createNDFA();
}

Regex::Expression *Regex::Union::clone() {
    return new Union(rhs->clone(), lhs->clone());
}

Regex::Concat::Concat(Regex::Expression *rhs, Regex::Expression *lhs)
        : Binary(rhs, lhs) {}

NDFA Regex::Concat::createNDFA() {
    return rhs->createNDFA() * lhs->createNDFA();
}

Regex::Expression *Regex::Concat::clone() {
    return new Concat(rhs->clone(), lhs->clone());
}

Regex::KleeneStar::KleeneStar(Regex::Expression *expression)
        : expr(expression) {}

NDFA Regex::KleeneStar::createNDFA() {
    return *expr->createNDFA();
}

Regex::Expression *Regex::KleeneStar::clone() {
    return new KleeneStar(expr->clone());
}

Regex::KleeneStar::~KleeneStar() {
    delete expr;
}


Regex::Word::Word(const String &word) : word(word) {}

Regex::Word::Word(String &&word) : word(std::move(word)) {}

NDFA Regex::Word::createNDFA() {
    return NDFAFactory::exact(word.c_str());
}

Regex::Expression *Regex::Word::clone() {
    return new Word(word);
}

NDFA Regex::EmptyWord::createNDFA() {
    return NDFAFactory::emptyWord();
}

Regex::Expression *Regex::EmptyWord::clone() {
    return new EmptyWord();
}

NDFA Regex::EmptyLanguage::createNDFA() {
    return NDFAFactory::emptyLanguage();
}

Regex::Expression *Regex::EmptyLanguage::clone() {
    return new EmptyLanguage();
}
