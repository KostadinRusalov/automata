#pragma once

#include "../Vector/Vector.hpp"
#include "../Algorithm.hpp"

template<class T>
class Set {
public:
    typedef T value_type;
    typedef typename Vector<T>::size_type size_type;
    typedef typename Vector<T>::iterator iterator;
    typedef typename Vector<T>::const_iterator const_iterator;
private:
    Vector<T> elements;
public:
    Set() = default;

    Set(const Vector<T> &elements);

    Set(Vector<T> &&elements);

    Set(std::initializer_list<value_type> data);

    bool empty() const;

    size_type size() const;

    size_type capacity() const;

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
};

template<class T>
Set<T>::Set(const Vector<T> &elements) {
    for (auto &element: elements) {
        add(element);
    }
}

template<class T>
Set<T>::Set(Vector<T> &&elements) {
    for (auto &element: elements) {
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
    return elements.empty();
}

template<class T>
typename Set<T>::size_type Set<T>::size() const {
    return elements.size();
}

template<class T>
typename Set<T>::size_type Set<T>::capacity() const {
    return elements.capacity();
}

template<class T>
void Set<T>::clear() {
    elements.clear();
}

template<class T>
void Set<T>::add(const T &element) {
    if (!contains(element)) {
        elements.pushBack(element);
    }
}

template<class T>
void Set<T>::add(T &&element) {
    if (!contains(element)) {
        elements.pushBack(element);
    }
}

template<class T>
void Set<T>::remove(const T &element) {
    const_iterator pos = cbegin() + (find(element) - begin());
    if (pos != cend()) {
        elements.erase(pos);
    }
}

template<class T>
bool Set<T>::contains(const T &element) const {
    return find(element) != cend();
}

template<class T>
typename Set<T>::iterator Set<T>::find(const T &element) {
    return kstd::find(elements.begin(), elements.end(), element);
}

template<class T>
typename Set<T>::const_iterator Set<T>::find(const T &element) const {
    return kstd::find(elements.cbegin(), elements.cend(), element);
}

template<class T>
typename Set<T>::iterator Set<T>::begin() {
    return elements.begin();
}

template<class T>
typename Set<T>::const_iterator Set<T>::begin() const {
    return elements.begin();
}

template<class T>
typename Set<T>::const_iterator Set<T>::cbegin() const {
    return elements.cbegin();
}

template<class T>
typename Set<T>::iterator Set<T>::end() {
    return elements.end();
}

template<class T>
typename Set<T>::const_iterator Set<T>::end() const {
    return elements.end();
}

template<class T>
typename Set<T>::const_iterator Set<T>::cend() const {
    return elements.cend();
}