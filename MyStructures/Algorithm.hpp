#pragma once

template<class Iter, class T>
Iter find(Iter first, Iter last, const T &value) {
    for (auto it = first; it != last; ++it) {
        if (value == *it) {
            return it;
        }
    }
    return last;
}