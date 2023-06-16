#pragma once

template<class T>
class CState {
    typedef T value_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;

protected:
    T label_;

public:
    CState() = default;

    CState(const T &label);

    reference label();

    const_reference label() const;
};

template<class T>
CState<T>::CState(const T &label) : label_{label} {}

template<class T>
typename CState<T>::reference CState<T>::label() {
    return label_;
}

template<class T>
typename CState<T>::const_reference CState<T>::label() const {
    return label_;
}

template<class T>
bool operator==(const CState<T> &rhs, const CState<T> &lhs) {
    return rhs.label() == lhs.label();
}

template<class T>
bool operator!=(const CState<T> &rhs, const CState<T> &lhs) {
    return !(rhs == lhs);
}