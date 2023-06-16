#include "Subset.h"
#include <utility>

Subset::Subset() : Subset(BUCKET_SIZE - 1) {}

Subset::Subset(value_type max)
        : max{max} {
    data = new Bucket[bucketCount()]{};
}

Subset::Subset(const Subset &other) {
    copyFrom(other);
}

Subset::Subset(Subset &&other) noexcept {
    moveFrom(std::move(other));
}

Subset &Subset::operator=(const Subset &other) {
    if (this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

Subset &Subset::operator=(Subset &&other) noexcept {
    if (this != &other) {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

Subset::~Subset() {
    free();
}

bool Subset::empty() const {
    for (size_type b = 0; b < bucketCount(); ++b) {
        if (data[b] != 0) {
            return false;
        }
    }
    return true;
}

Subset::value_type Subset::capacity() const {
    return max;
}

void Subset::clear() {
    for (size_type b = 0; b < bucketCount(); ++b) {
        data[b] = 0;
    }
}

void Subset::add(Subset::value_type num) {
    if (num >= max) {
        resize(num + 1);
    }

    data[bucket(num)] |= position(num);

}

void Subset::remove(Subset::value_type num) {
    if (num >= max) {
        return;
    }

    data[bucket(num)] &= ~position(num);
}

bool Subset::contains(Subset::value_type num) const {
    if (num >= max) {
        return false;
    }

    return data[bucket(num)] & position(num);
}

Subset Subset::operator~() const {
    Subset complement(*this);
    complement.flip();
    return complement;
}

void Subset::flip() {
    size_type buckets = bucketCount();
    for (size_type b = 0; b < buckets; ++b) {
        data[b] = ~data[b];
    }
}

void Subset::free() {
    delete[] data;
    data = nullptr;
}

void Subset::copyFrom(const Subset &other) {
    max = other.max;

    size_type buckets = bucketCount();
    data = new Bucket[buckets];
    for (size_type b = 0; b < buckets; ++b) {
        data[b] = other.data[b];
    }
}

void Subset::moveFrom(Subset &&other) {
    max = other.max;

    data = other.data;
    other.data = nullptr;
}

void Subset::resize(Subset::value_type newMax) {
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

Subset::size_type Subset::bucketCount() const {
    return 1 + max / BUCKET_SIZE;
}

Subset::size_type Subset::bucket(Subset::value_type num) const {
    return num / BUCKET_SIZE;
}

Subset::position_idx Subset::position(Subset::value_type num) const {
    size_type shift = (BUCKET_SIZE - 1) - num % BUCKET_SIZE;
    return 1 << shift;
}

bool operator==(const Subset &rhs, const Subset &lhs) {
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

bool operator!=(const Subset &rhs, const Subset &lhs) {
    return !(rhs == lhs);
}
