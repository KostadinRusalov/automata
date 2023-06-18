#pragma once

#include <iostream>

class Command {
public:
    virtual ~Command() = default;

    virtual void run(std::ostream &strm) = 0;
};

class Quit : public Command {

};