#include "../include/Regex.h"
#include "../include/NDFAFactory.h"

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

NDFA Regex::toNDFA() const {
    return expr->toNDFA();
}

String Regex::toString() const {
    return expr->toString();
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

    size_t findInnermostBracket(const StringView &expr) {
        size_t idx = String::npos;
        for (size_t i = 0; i < expr.size(); ++i) {
            if (expr[i] == '(') {
                idx = i;
            } else if (expr[i] == ')') {
                return idx;
            }
        }
        return idx;
    }
}

Regex::Expression *Regex::simpleParse(const StringView &expr) {
    if (expr.empty()) {
        return new EmptyWord;
    }

    if (expr.size() == 1) {
        return new Letter(expr.front());
    }

    size_t uPos = expr.find((char) Symbol::Union);
    if (uPos != String::npos) {
        return new Union(simpleParse(expr.substr(0, uPos)),
                         simpleParse(expr.substr(uPos + 1)));
    }

    size_t cPos = expr.find((char) Symbol::CloseBracket);
    if (cPos != String::npos) {
        return new Concat(simpleParse(expr.substr(0, cPos)),
                          simpleParse(expr.substr(cPos + 1)));
    }

    size_t sPos = expr.find((char) Symbol::Star);
    if (sPos != String::npos) {
        return new KleeneStar(simpleParse(expr.substr(0, sPos)));
    }

}

Regex::Expression *Regex::parse(const StringView &expr) {
    if (expr.empty()) {
        return new EmptyWord;
    }

    if (expr.size() == 1) {
        return new Letter(expr.front());
    }

    if (expr.front() == (char) Symbol::OpenBracket) {
        size_t balanced = findBalancedBracket(0, expr);
        if (balanced == String::npos) {
            throw std::invalid_argument("invalid expression!");
        }

        size_t count = balanced - 1;
        if (balanced + 1 == expr.size()) {
            return parse(expr.substr(1, count));
        }

        switch (expr[balanced + 1]) {
            case (char) Symbol::Star:
                if (balanced + 2 == expr.size()) {
                    return new KleeneStar(parse(expr.substr(1, count)));
                } else {
                    switch (expr[balanced + 2]) {
                        case (char) Symbol::Star:
                        case (char) Symbol::CloseBracket:
                            throw std::invalid_argument("invalid expression!");
                        case (char) Symbol::Union:
                            return new Union(new KleeneStar(parse(expr.substr(1, count))),
                                             parse(expr.substr(balanced + 2)));
                        default:
                            return new Concat(new KleeneStar(parse(expr.substr(1, count))),
                                              parse(expr.substr(balanced + 2)));
                    }
                }
            case (char) Symbol::Union:
                return new Union(parse(expr.substr(1, count)), parse(expr.substr(balanced + 2)));
            case (char) Symbol::CloseBracket:
                throw std::invalid_argument("invalid expression!");
            default:
                return new Concat(parse(expr.substr(1, count)), parse(expr.substr(balanced + 1)));
        }
    }

    return new Concat(parse(expr.substr(0, 1)), parse(expr.substr(1)));
}

Regex::Binary::Binary(Expression *rhs, Expression *lhs)
        : rhs(rhs), lhs(lhs) {}

Regex::Binary::~Binary() {
    delete rhs;
    delete lhs;
}

Regex::Union::Union(Regex::Expression *rhs, Regex::Expression *lhs)
        : Binary(rhs, lhs) {}

NDFA Regex::Union::toNDFA() const {
    return rhs->toNDFA() | lhs->toNDFA();
}

String Regex::Union::toString() const {
    return "(" + rhs->toString() + "+" + lhs->toString() + ")";
}

Regex::Expression *Regex::Union::clone() const {
    return new Union(rhs->clone(), lhs->clone());
}

Regex::Concat::Concat(Regex::Expression *rhs, Regex::Expression *lhs)
        : Binary(rhs, lhs) {}

NDFA Regex::Concat::toNDFA() const {
    return rhs->toNDFA() * lhs->toNDFA();
}

String Regex::Concat::toString() const {
    return rhs->toString() + lhs->toString();
}

Regex::Expression *Regex::Concat::clone() const {
    return new Concat(rhs->clone(), lhs->clone());
}

Regex::KleeneStar::KleeneStar(Regex::Expression *expression)
        : expr(expression) {}

NDFA Regex::KleeneStar::toNDFA() const {
    return *expr->toNDFA();
}

String Regex::KleeneStar::toString() const {
    return "(" + expr->toString() + ")*";
}

Regex::Expression *Regex::KleeneStar::clone() const {
    return new KleeneStar(expr->clone());
}

Regex::KleeneStar::~KleeneStar() {
    delete expr;
}

Regex::Letter::Letter(char
                      letter) : letter(letter) {}

NDFA Regex::Letter::toNDFA() const {
    return NDFAFactory::exact(letter);
}

String Regex::Letter::toString() const {
    String s;
    s += letter;
    return s;
}

Regex::Expression *Regex::Letter::clone() const {
    return new Letter(letter);
}

NDFA Regex::EmptyWord::toNDFA() const {
    return NDFAFactory::emptyWord();
}

String Regex::EmptyWord::toString() const {
    return {""};
}

Regex::Expression *Regex::EmptyWord::clone() const {
    return new EmptyWord();
}

NDFA Regex::EmptyLanguage::toNDFA() const {
    return NDFAFactory::emptyLanguage();
}

String Regex::EmptyLanguage::toString() const {
    return {"\\O"};
}

Regex::Expression *Regex::EmptyLanguage::clone() const {
    return new EmptyLanguage();
}
