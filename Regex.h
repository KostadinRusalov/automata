#pragma once

#include "NDFA.h"
#include "MyStructures/StringView/StringView.h"

class Regex {
public:
    enum class Symbol;

    class Expression;

    class Binary;

    class Union;

    class Concat;

    class KleeneStar;

    class Letter;

    class Word;

    class EmptyWord;

    class EmptyLanguage;

    Expression *expr;

public:
    Regex();

    Regex(Expression *expression);

    Regex(const StringView &regex);

    Regex(const Regex &other);

    Regex(Regex &&other) noexcept;

    Regex &operator=(const Regex &other);

    Regex &operator=(Regex &&other) noexcept;

    ~Regex();

    NDFA createNDFA() const;

private:
    void free();

    void copyFrom(const Regex &other);

    void moveFrom(Regex &&other);

    static Expression *parse(const StringView &expression);
};

enum class Regex::Symbol {
    OpenBracket = '(',
    CloseBracket = ')',
    Union = '+',
    Concat = '.',
    Star = '*'
};

class Regex::Expression {
public:
    virtual NDFA createNDFA() = 0;

    virtual Expression *clone() = 0;

    virtual ~Expression() = default;
};

class Regex::Binary : public Regex::Expression {
protected:
    Expression *rhs;
    Expression *lhs;

public:
    Binary(Expression *rhs, Expression *lhs);

    ~Binary() override;
};

class Regex::Union : public Regex::Binary {
public:
    Union(Expression *rhs, Expression *lhs);

    NDFA createNDFA() override;

    Expression *clone() override;

    ~Union() override = default;
};

class Regex::Concat : public Regex::Binary {
public:
    Concat(Expression *rhs, Expression *lhs);

    NDFA createNDFA() override;

    Expression *clone() override;

    ~Concat() override = default;
};

class Regex::KleeneStar : public Regex::Expression {
private:
    Expression *expr;

public:
    KleeneStar(Expression *expression);

    NDFA createNDFA() override;

    Expression *clone() override;

    ~KleeneStar() override;
};

class Regex::Letter : public Regex::Expression {
private:
    char letter;

public:
    Letter(char letter);

    NDFA createNDFA() override;

    Expression *clone() override;

    ~Letter() override = default;
};

class Regex::Word : public Regex::Expression {
private:
    StringView word;

public:
    Word(const StringView &word);

    NDFA createNDFA() override;

    Expression *clone() override;

    ~Word() override = default;
};

class Regex::EmptyWord : public Regex::Expression {
public:
    EmptyWord() = default;

    NDFA createNDFA() override;

    Expression *clone() override;

    ~EmptyWord() override = default;
};

class Regex::EmptyLanguage : public Regex::Expression {
public:
    EmptyLanguage() = default;

    NDFA createNDFA() override;

    Expression *clone() override;

    ~EmptyLanguage() override = default;
};
