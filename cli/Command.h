#pragma once

#include <iostream>
#include "../MyStructures/StringView/StringView.h"

class Command {
public:

    virtual ~Command() = default;

    virtual void run() = 0;
};

class Quit : public Command {

};

class CreateNDFA : public Command {
public:
    explicit CreateNDFA(const StringView &regex);

    explicit CreateNDFA(StringView &&regex);

    void run() override;

    ~CreateNDFA() override = default;
};

class Determinize : public Command {
};

class Totalize : public Command {
};

class Accepts : public Command {

};

class IsEmptyLanguage : public Command {

};

class Union : public Command {

};

class Concat : public Command {

};

class KleeneStar : public Command {

};