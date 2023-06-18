#include "Regex.h"
#include "NDFAFactory.h"

Regex::Regex() {
    expr = new EmptyLanguage();
}

Regex::Regex(Regex::Expression *expression) : expr(expression) {}

Regex::Regex(const StringView &regex) {
    expr = parse(regex);
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

NDFA Regex::createNDFA() const {
    return expr->createNDFA();
}

void Regex::free() {
    delete expr;
}

void Regex::copyFrom(const Regex &other) {
    expr = other.expr->clone();
}

void Regex::moveFrom(Regex &&other) {
    expr = other.expr;
    other.expr = nullptr;
}

namespace {
    size_t findBalancedBracket(size_t from, const StringView &expr) {
        int count = 1;
        for (size_t c = from + 1; c < expr.size(); ++c) {
            if (expr[c] == '(') {
                count++;
            } else if (expr[c] == ')') {
                count--;
            }

            if (count == 0) {
                return c;
            }
        }
        return String::npos;
    }
}

Regex::Expression *Regex::parse(const StringView &expr) {
    if (expr.empty()) {
        return new EmptyWord;
    }

    if (expr.size() == 1) {
        return new Letter(expr.front());
    }

    size_t uPos = expr.find('+');
    if (uPos != String::npos) {
        return new Union(parse(expr.substr(0, uPos)),
                         parse(expr.substr(uPos + 1)));
    }

    size_t cPos = expr.find('.');
    if (cPos != String::npos) {
        return new Concat(parse(expr.substr(0, cPos)),
                          parse(expr.substr(cPos + 1)));
    }

    size_t sPos = expr.find('*');
    if (sPos != String::npos) {
        return new KleeneStar(parse(expr.substr(0, sPos)));
    }

    return new Word(expr);
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

Regex::Word::Word(const StringView &word) : word(word) {}

NDFA Regex::Word::createNDFA() {
    return NDFAFactory::exact(word);
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
