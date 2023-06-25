cmake_minimum_required(VERSION 3.23)
project(automata)

set(CMAKE_CXX_STANDARD 14)

add_executable(main.cpp
        include/Automata.h src/Automata.cpp include/CDFA.hpp include/CState.hpp src/DFA.cpp include/DFA.h include/NDFA.h src/NDFA.cpp
        src/NDFAFactory.cpp include/NDFAFactory.h include/helpers/SubtractOneAfter.hpp
        MyStructures/Vector/Vector.hpp
        MyStructures/Pair/Pair.hpp
        MyStructures/BitSubset/BitSubset.h MyStructures/BitSubset/BitSubset.cpp
        MyStructures/Queue/Queue.hpp
        MyStructures/String/String.h MyStructures/String/String.cpp
        MyStructures/String/BitManipulation.cpp
        MyStructures/String/BitManipulation.h
        MyStructures/Algorithm.hpp
        src/Regex.cpp include/Regex.h
        MyStructures/ArrayIterator/ArrayIterator.hpp
        MyStructures/StringView/StringView.cpp
        MyStructures/StringView/StringView.h src/GNFA.cpp include/GNFA.h include/helpers/File.hpp main.cpp cli/Command.h tests/test.h cli/Command.cpp cli/CommandFactory.cpp cli/CommandFactory.h cli/UI.cpp cli/UI.h)
