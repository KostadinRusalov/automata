#include "Regex.h"
#include "NDFAFactory.h"

Regex::Regex() {
    expression = new EmptyLanguage();
}

Regex::Regex(const StringView &regex) {
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

Regex::Expression *Regex::parse(const StringView &expr) {
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

Regex::Letter::Letter(char letter) : letter(letter) {}

NDFA Regex::Letter::createNDFA() {
    return NDFAFactory::exact(letter);
}

Regex::Expression *Regex::Letter::clone() {
    return new Letter(letter);
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
