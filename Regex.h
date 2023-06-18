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

    NDFA toNDFA() const;

    String toString() const;

private:
    void free();

    void copyFrom(const Regex &other);

    void moveFrom(Regex &&other);

    static Expression *simpleParse(const StringView &expr);

    static Expression *parse(const StringView &expr);
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
    virtual NDFA toNDFA() const = 0;

    virtual String toString() const = 0;

    virtual Expression *clone() const = 0;

    virtual bool isSimple() const = 0;

    virtual ~Expression() = default;

    static String addBrackets(Expression *expr);
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

    NDFA toNDFA() const override;

    String toString() const override;

    Expression *clone() const override;

    bool isSimple() const override;

    ~Union() override = default;
};

class Regex::Concat : public Regex::Binary {
public:
    Concat(Expression *rhs, Expression *lhs);

    NDFA toNDFA() const override;

    String toString() const override;

    Expression *clone() const override;

    bool isSimple() const override;

    ~Concat() override = default;
};

class Regex::KleeneStar : public Regex::Expression {
private:
    Expression *expr;

public:
    KleeneStar(Expression *expression);

    NDFA toNDFA() const override;

    String toString() const override;

    Expression *clone() const override;

    bool isSimple() const override;

    ~KleeneStar() override;
};

class Regex::Letter : public Regex::Expression {
private:
    char letter;

public:
    Letter(char letter);

    NDFA toNDFA() const override;

    String toString() const override;

    Expression *clone() const override;

    bool isSimple() const override;

    ~Letter() override = default;
};

class Regex::Word : public Regex::Expression {
private:
    StringView word;

public:
    Word(const StringView &word);

    NDFA toNDFA() const override;

    String toString() const override;

    Expression *clone() const override;

    bool isSimple() const override;

    ~Word() override = default;
};

class Regex::EmptyWord : public Regex::Expression {
public:
    EmptyWord() = default;

    NDFA toNDFA() const override;

    String toString() const override;

    Expression *clone() const override;

    bool isSimple() const override;

    ~EmptyWord() override = default;
};

class Regex::EmptyLanguage : public Regex::Expression {
public:
    EmptyLanguage() = default;

    NDFA toNDFA() const override;

    String toString() const override;

    Expression *clone() const override;

    bool isSimple() const override;

    ~EmptyLanguage() override = default;
};
