#pragma once

#include "NDFA.h"
#include "MyStructures/String/String.h"

class Regex {
private:
    class Expression;

    class Binary;

    class Union;

    class Concat;

    class KleeneStar;

    class Word;

    class EmptyWord;

    class EmptyLanguage;

    Expression *expression;

public:
    Regex();

    Regex(const String &regex);

    Regex(const Regex &other);

    Regex(Regex &&other) noexcept;

    Regex &operator=(const Regex &other);

    Regex &operator=(Regex &&other) noexcept;

    ~Regex();

private:
    void free();

    void copyFrom(const Regex &other);

    void moveFrom(Regex &&other);

    static Expression *parse(const String &expression);
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

class Regex::Word : public Regex::Expression {
private:
    String word;

public:
    Word(const String &word);

    Word(String &&word);

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
