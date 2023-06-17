#pragma once

#include <cstddef>
#include <cstdint>
#include "../Vector/Vector.hpp"

class BitSubset {
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
    BitSubset();

    explicit BitSubset(value_type max);

    explicit BitSubset(const Vector<unsigned> &collection);

    BitSubset(const BitSubset &other);

    BitSubset(BitSubset &&other) noexcept;

    BitSubset &operator=(const BitSubset &other);

    BitSubset &operator=(BitSubset &&other) noexcept;

    ~BitSubset();

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

    BitSubset operator~() const;

    void flip();

private:
    void free();

    void copyFrom(const BitSubset &other);

    void moveFrom(BitSubset &&other);

    void resize(value_type newMax);

    size_type bucketCount() const;

    size_type bucket(value_type num) const;

    position_idx position(value_type num) const;

    friend bool operator==(const BitSubset &rhs, const BitSubset &lhs);

    friend bool operator!=(const BitSubset &rhs, const BitSubset &lhs);

};

template<class Container>
void BitSubset::add(const Container &nums) {
    for (auto num: nums) {
        add(num);
    }
}