#include "BitSubset.h"
#include <utility>

BitSubset::BitSubset() : BitSubset(BUCKET_SIZE - 1) {}

BitSubset::BitSubset(value_type max)
        : max{max} {
    data = new Bucket[bucketCount()]{};
}

BitSubset::BitSubset(const Vector<unsigned> &collection) : BitSubset() {
    for (auto num: collection) {
        add(num);
    }
}

BitSubset::BitSubset(const BitSubset &other) {
    copyFrom(other);
}

BitSubset::BitSubset(BitSubset &&other) noexcept {
    moveFrom(std::move(other));
}

BitSubset &BitSubset::operator=(const BitSubset &other) {
    if (this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

BitSubset &BitSubset::operator=(BitSubset &&other) noexcept {
    if (this != &other) {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

BitSubset::~BitSubset() {
    free();
}

bool BitSubset::empty() const {
    for (size_type b = 0; b < bucketCount(); ++b) {
        if (data[b] != 0) {
            return false;
        }
    }
    return true;
}

BitSubset::value_type BitSubset::capacity() const {
    return max;
}

void BitSubset::clear() {
    for (size_type b = 0; b < bucketCount(); ++b) {
        data[b] = 0;
    }
}

void BitSubset::add(BitSubset::value_type num) {
    if (num >= max) {
        resize(num + 1);
    }

    data[bucket(num)] |= position(num);

}

void BitSubset::remove(BitSubset::value_type num) {
    if (num >= max) {
        return;
    }

    data[bucket(num)] &= ~position(num);
}

bool BitSubset::contains(BitSubset::value_type num) const {
    if (num >= max) {
        return false;
    }

    return data[bucket(num)] & position(num);
}

BitSubset BitSubset::operator~() const {
    BitSubset complement(*this);
    complement.flip();
    return complement;
}

void BitSubset::flip() {
    size_type buckets = bucketCount();
    for (size_type b = 0; b < buckets; ++b) {
        data[b] = ~data[b];
    }
}

void BitSubset::free() {
    delete[] data;
    data = nullptr;
}

void BitSubset::copyFrom(const BitSubset &other) {
    max = other.max;

    size_type buckets = bucketCount();
    data = new Bucket[buckets];
    for (size_type b = 0; b < buckets; ++b) {
        data[b] = other.data[b];
    }
}

void BitSubset::moveFrom(BitSubset &&other) {
    max = other.max;

    data = other.data;
    other.data = nullptr;
}

void BitSubset::resize(BitSubset::value_type newMax) {
    if (bucket(newMax) <= bucketCount()) {
        max = newMax;
        return;
    }

    size_type oldCount = bucketCount();
    max = newMax;
    auto temp = new Bucket[bucketCount()]{};
    for (size_type b = 0; b < oldCount; ++b) {
        temp[b] = data[b];
    }

    free();
    data = temp;
}

BitSubset::size_type BitSubset::bucketCount() const {
    return 1 + max / BUCKET_SIZE;
}

BitSubset::size_type BitSubset::bucket(BitSubset::value_type num) const {
    return num / BUCKET_SIZE;
}

BitSubset::position_idx BitSubset::position(BitSubset::value_type num) const {
    size_type shift = (BUCKET_SIZE - 1) - num % BUCKET_SIZE;
    return 1 << shift;
}

bool operator==(const BitSubset &rhs, const BitSubset &lhs) {
    size_t rCount = rhs.bucketCount();
    size_t lCount = lhs.bucketCount();
    size_t min = rCount < lCount ? rCount : lCount;

    for (size_t b = 0; b < min; ++b) {
        if (rhs.data[b] != lhs.data[b]) {
            return false;
        }
    }

    for (size_t b = min; b < lCount; ++b) {
        if (lhs.data[b] > 0) {
            return false;
        }
    }

    return true;
}

bool operator!=(const BitSubset &rhs, const BitSubset &lhs) {
    return !(rhs == lhs);
}
