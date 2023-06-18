#pragma once

template<class T>
class CState {
    typedef T value_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;

protected:
    T data_;

public:
    CState() = default;

    CState(const T &data);

    reference data();

    const_reference data() const;
};

template<class T>
CState<T>::CState(const T &data) : data_{data} {}

template<class T>
typename CState<T>::reference CState<T>::data() {
    return data_;
}

template<class T>
typename CState<T>::const_reference CState<T>::data() const {
    return data_;
}

template<class T>
bool operator==(const CState<T> &rhs, const CState<T> &lhs) {
    return rhs.data() == lhs.data();
}

template<class T>
bool operator!=(const CState<T> &rhs, const CState<T> &lhs) {
    return !(rhs == lhs);
}