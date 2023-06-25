#pragma once

template<size_t N, class Num>
class Subtract {
private:
    Num after;
public:
    explicit Subtract(const Num &after);

    Num operator()(const Num &num);
};

template<size_t N, class Num>
Subtract<N, Num>::Subtract(const Num &after) : after(after) {}

template<size_t N, class Num>
Num Subtract<N, Num>::operator()(const Num &num) {
    return num >= after ? num - N : num;
}

template<class T, class Collection>
void removeAndShift(const T &element, Collection &collection) {
    collection.remove(element);
    kstd::transform(collection.begin(), collection.end(),
                    collection.begin(), Subtract<1, T>(element));
}