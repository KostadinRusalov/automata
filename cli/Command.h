#pragma once

#include <iostream>
#include "../MyStructures/StringView/StringView.h"
#include "../include/Regex.h"

class Command {
protected:
    static Vector<NDFA> automatas;

    bool validate(size_t idx) const;

public:
    virtual ~Command() = default;

    virtual void run() = 0;
};

class CreateNDFA : public Command {
private :
    Regex regex;
public:
    explicit CreateNDFA(const StringView &regex);

    void run() override;

    ~CreateNDFA() override = default;
};

class Determinize : public Command {
private:
    size_t idx;
public:
    explicit Determinize(size_t idx);

    void run() override;

    ~Determinize() override = default;
};

class Totalize : public Command {
private:
    size_t idx;
public:
    explicit Totalize(size_t idx);

    void run() override;

    ~Totalize() override = default;
};

class Accepts : public Command {
private:
    size_t idx;
    String word;
public:
    Accepts(size_t idx, const String &word);

    void run() override;

    ~Accepts() override = default;
};

class IsEmptyLanguage : public Command {
private:
    size_t idx;

public:
    explicit IsEmptyLanguage(size_t idx);

    void run() override;

    ~IsEmptyLanguage() override = default;
};

class Union : public Command {
private:
    size_t rhs;
    size_t lhs;

public:
    Union(size_t rhs, size_t lhs);

    void run() override;

    ~Union() override = default;
};

class Concat : public Command {
private:
    size_t rhs;
    size_t lhs;

public:
    Concat(size_t rhs, size_t lhs);

    void run() override;

    ~Concat() override = default;
};

class KleeneStar : public Command {
private:
    size_t idx;

public:
    explicit KleeneStar(size_t idx);

    void run() override;

    ~KleeneStar() override = default;
};

class Help : public Command {
public:
    Help() = default;

    void run() override;

    ~Help() override = default;
};