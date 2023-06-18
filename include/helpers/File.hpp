#pragma once

#include "../../MyStructures/Set/Set.hpp"
#include "../../MyStructures/Vector/Vector.hpp"

#include <fstream>

namespace File {
    template<class T>
    void readSet(std::ifstream &binaryFile, Set<T> &set) {
        size_t size;
        binaryFile.read((char *) &size, sizeof(size));

        auto buff = new T[size];
        binaryFile.read((char *) buff, sizeof(T) * size);

        for (size_t i = 0; i < size; ++i) {
            set.add(buff[i]);
        }

        delete[] buff;
    }

    template<class T>
    void saveSet(std::ofstream &binaryFile, const Set<T> &set) {
        size_t size = set.size();
        binaryFile.write((const char *) &size, sizeof(size));
        binaryFile.write((const char *) set.elements().data(), sizeof(T) * size);
    }

    template<class T>
    void readVector(std::ifstream &binaryFile, Vector<T> &vec) {
        size_t size;
        binaryFile.read((char *) &size, sizeof(size));

        auto buff = new T[size];
        binaryFile.read((char *) buff, sizeof(T) * size);

        for (size_t i = 0; i < size; ++i) {
            vec.pushBack(buff[i]);
        }

        delete[] buff;
    }

    template<class T>
    void saveVector(std::ofstream &binaryFile, const Vector<T> &vec) {
        size_t size = vec.size();
        binaryFile.write((const char *) &size, sizeof(size));
        binaryFile.write((const char *) vec.data(), sizeof(T) * size);
    }
}
