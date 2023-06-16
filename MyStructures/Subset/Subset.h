#pragma once

#include <cstddef>
#include <cstdint>

class Subset {
public:
    typedef uint32_t Bucket;
    typedef size_t size_type;
    typedef unsigned value_type;
    typedef size_type position_idx;

    static const unsigned short ONE_BYTE = 8;
    static const size_type BUCKET_SIZE = ONE_BYTE * sizeof(Bucket);

private:
    Bucket *data;
    value_type max;
    mutable value_type curr;

public:
    Subset();

    explicit Subset(value_type max);

    Subset(const Subset &other);

    Subset(Subset &&other) noexcept;

    Subset &operator=(const Subset &other);

    Subset &operator=(Subset &&other) noexcept;

    ~Subset();

    // capacity
    bool empty() const;

    value_type capacity() const;

    // modifiers
    void clear();

    void add(value_type num);

    template<class Container>
    void add(const Container &num);

    void remove(value_type num);

    // element access
    bool contains(value_type num) const;

    Subset operator~() const;

    void flip();

private:
    void free();

    void copyFrom(const Subset &other);

    void moveFrom(Subset &&other);

    void resize(value_type newMax);

    size_type bucketCount() const;

    size_type bucket(value_type num) const;

    position_idx position(value_type num) const;

    friend bool operator==(const Subset &rhs, const Subset &lhs);

    friend bool operator!=(const Subset &rhs, const Subset &lhs);

};

template<class Container>
void Subset::add(const Container &nums) {
    for (auto num: nums) {
        add(num);
    }
}