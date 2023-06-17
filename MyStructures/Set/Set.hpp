#pragma once

#include "../Vector/Vector.hpp"
#include "../Algorithm.hpp"
#include "../BitSubset/BitSubset.h"

template<class T>
class Set {
public:
    typedef T value_type;
    typedef typename Vector<T>::size_type size_type;
    typedef typename Vector<T>::iterator iterator;
    typedef typename Vector<T>::const_iterator const_iterator;

private:
    Vector<T> elements_;
public:
    Set() = default;

    Set(const Vector<T> &elements_);

    Set(Vector<T> &&elements_);

    Set(std::initializer_list<value_type> data);

    bool empty() const;

    size_type size() const;

    size_type capacity() const;

    const Vector<T> &elements() const;

    void clear();

    void add(const T &element);

    void add(T &&element);

    void remove(const T &element);

    bool contains(const T &element) const;

    iterator find(const T &element);

    const_iterator find(const T &element) const;

    iterator begin();

    const_iterator begin() const;

    const_iterator cbegin() const;

    iterator end();

    const_iterator end() const;

    const_iterator cend() const;

    bool intersectsWith(const Set<T> &other) const;
};

template<class T>
Set<T>::Set(const Vector<T> &elements_) {
    for (auto &element: elements_) {
        add(element);
    }
}

template<class T>
Set<T>::Set(Vector<T> &&elements_) {
    for (auto &element: elements_) {
        add(std::move(element));
    }
}

template<class T>
Set<T>::Set(std::initializer_list<value_type> data) {
    for (auto &element: data) {
        add(element);
    }
}

template<class T>
bool Set<T>::empty() const {
    return elements_.empty();
}

template<class T>
typename Set<T>::size_type Set<T>::size() const {
    return elements_.size();
}

template<class T>
typename Set<T>::size_type Set<T>::capacity() const {
    return elements_.capacity();
}

template<class T>
void Set<T>::clear() {
    elements_.clear();
}

template<class T>
void Set<T>::add(const T &element) {
    if (!contains(element)) {
        elements_.pushBack(element);
    }
}

template<class T>
void Set<T>::add(T &&element) {
    if (!contains(element)) {
        elements_.pushBack(element);
    }
}

template<class T>
void Set<T>::remove(const T &element) {
    const_iterator pos = cbegin() + (find(element) - begin());
    if (pos != cend()) {
        elements_.erase(pos);
    }
}

template<class T>
bool Set<T>::contains(const T &element) const {
    return find(element) != cend();
}

template<class T>
typename Set<T>::iterator Set<T>::find(const T &element) {
    return kstd::find(elements_.begin(), elements_.end(), element);
}

template<class T>
typename Set<T>::const_iterator Set<T>::find(const T &element) const {
    return kstd::find(elements_.cbegin(), elements_.cend(), element);
}

template<class T>
typename Set<T>::iterator Set<T>::begin() {
    return elements_.begin();
}

template<class T>
typename Set<T>::const_iterator Set<T>::begin() const {
    return elements_.begin();
}

template<class T>
typename Set<T>::const_iterator Set<T>::cbegin() const {
    return elements_.cbegin();
}

template<class T>
typename Set<T>::iterator Set<T>::end() {
    return elements_.end();
}

template<class T>
typename Set<T>::const_iterator Set<T>::end() const {
    return elements_.end();
}

template<class T>
typename Set<T>::const_iterator Set<T>::cend() const {
    return elements_.cend();
}

template<class T>
bool Set<T>::intersectsWith(const Set<T> &other) const {
    BitSubset oth(other.elements_);
    for (auto &el: elements_) {
        if (oth.contains(el)) {
            return true;
        }
    }
    return false;
}

template<class T>
const Vector<T> &Set<T>::elements() const {
    return elements_;
}
