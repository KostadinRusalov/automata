#pragma once

#include <cstddef>
#include <cstdint>

class Bitset {
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
    explicit Bitset(value_type max);

    Bitset(const Bitset &other);

    Bitset(Bitset &&other) noexcept;

    Bitset &operator=(const Bitset &other);

    Bitset &operator=(Bitset &&other) noexcept;

    ~Bitset();

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

    Bitset operator~() const;

    void flip();

private:
    void free();

    void copyFrom(const Bitset &other);

    void moveFrom(Bitset &&other);

    void resize(value_type newMax);

    size_type bucketCount() const;

    size_type bucket(value_type num) const;

    position_idx position(value_type num) const;

    friend bool operator==(const Bitset &rhs, const Bitset &lhs);

    friend bool operator!=(const Bitset &rhs, const Bitset &lhs);

};

template<class Container>
void Bitset::add(const Container &nums) {
    for (auto num: nums) {
        add(num);
    }
}