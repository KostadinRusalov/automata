#pragma once

template<class Num>
class SubtractOneAfter {
private:
    Num after;
public:
    explicit SubtractOneAfter(Num after);

    Num operator()(Num num);
};

template<class Num>
SubtractOneAfter<Num>::SubtractOneAfter(Num after) : after(after) {}

template<class Num>
Num SubtractOneAfter<Num>::operator()(Num num) {
    return num > after ? --num : num;
}

template<class T, class Collection>
void removeAndShift(T element, Collection &collection) {
    collection.remove(element);
    kstd::transform(collection.begin(), collection.end(),
                    collection.begin(), SubtractOneAfter<T>(element));
}