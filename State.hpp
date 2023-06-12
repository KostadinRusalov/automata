#pragma once

#include <utility>

template<class T>
class State {
private:
    T label;

public:
    State() = default;

    State(const T &label);

    State(T &&label);
};

template<class T>
State<T>::State(const T &label)
        : label{label} {}

template<class T>
State<T>::State(T &&label)
        : label{std::move(label)} {}