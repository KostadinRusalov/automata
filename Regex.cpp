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

    size_t cPos = expr.find((char) Symbol::Concat);
    if (cPos != String::npos) {
        return new Concat(simpleParse(expr.substr(0, cPos)),
                          simpleParse(expr.substr(cPos + 1)));
    }

    size_t sPos = expr.find((char) Symbol::Star);
    if (sPos != String::npos) {
        return new KleeneStar(simpleParse(expr.substr(0, sPos)));
    }

    return new Word(expr);
}

Regex::Expression *Regex::parse(const StringView &expr) {
    if (expr.empty()) {
        return new EmptyWord;
    }

    if (expr.size() == 1) {
        return new Letter(expr.front());
    }


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

String Regex::Expression::addBrackets(Regex::Expression *expr) {
    if (expr->isSimple()) {
        return expr->toString();
    }
    return "(" + expr->toString() + ")";
}

String Regex::Union::toString() const {
    return Expression::addBrackets(rhs) + "+" + Expression::addBrackets(lhs);
}

bool Regex::Union::isSimple() const {
    return false;
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
    return Expression::addBrackets(rhs) +"." + Expression::addBrackets(lhs);
}

Regex::Expression *Regex::Concat::clone() const {
    return new Concat(rhs->clone(), lhs->clone());
}

bool Regex::Concat::isSimple() const {
    return false;
}

Regex::KleeneStar::KleeneStar(Regex::Expression *expression)
        : expr(expression) {}

NDFA Regex::KleeneStar::toNDFA() const {
    return *expr->toNDFA();
}

String Regex::KleeneStar::toString() const {
    return Expression::addBrackets(expr) + "*";
}

Regex::Expression *Regex::KleeneStar::clone() const {
    return new KleeneStar(expr->clone());
}

bool Regex::KleeneStar::isSimple() const {
    return true;
}

Regex::KleeneStar::~KleeneStar() {
    delete expr;
}

Regex::Letter::Letter(char letter) : letter(letter) {}

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

bool Regex::Letter::isSimple() const {
    return true;
}

Regex::Word::Word(const StringView &word) : word(word) {}

NDFA Regex::Word::toNDFA() const {
    return NDFAFactory::exact(word);
}

String Regex::Word::toString() const {
    String s(word.size());
    for (char c: word) {
        s += c;
    }
    return s;
}

Regex::Expression *Regex::Word::clone() const {
    return new Word(word);
}

bool Regex::Word::isSimple() const {
    return false;
}

NDFA Regex::EmptyWord::toNDFA() const {
    return NDFAFactory::emptyWord();
}

String Regex::EmptyWord::toString() const {
    return {"\\e"};
}

Regex::Expression *Regex::EmptyWord::clone() const {
    return new EmptyWord();
}

bool Regex::EmptyWord::isSimple() const {
    return true;
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

bool Regex::EmptyLanguage::isSimple() const {
    return true;
}