#pragma once

#include "../include/DFA.h"
#include "../include/NDFA.h"
#include "../include/GNFA.h"
#include "../include/NDFAFactory.h"
#include "../MyStructures/StringView/StringView.h"
#include "../include/Regex.h"
#include<fstream>

template<class Arr>
void print(const Arr &arr) {
    for (auto &el: arr) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}

void print(const GNFA &gnfa) {
    print(gnfa.alphabet());

    std::cout << "initial state: " << gnfa.initialState << std::endl;
    std::cout << "final state: " << gnfa.finalState << std::endl;

    for (size_t s = 0; s < gnfa.transitions.size(); ++s) {
        std::cout << "q_" << s << ":";
        for (auto &tr: gnfa.transitions[s]) {
            std::cout << " q_" << tr.first << " -> " << tr.second << ';';
        }
        std::cout << std::endl;
    }

}

void testUnreachableStates() {
    NDFA n({'a', 'b'});

    auto q0 = n.addInitialState();
    auto q1 = n.addFinalState();
    auto q2 = n.addState();
    auto q3 = n.addState();
    auto q4 = n.addState();
    auto q5 = n.addState();
    auto q6 = n.addState();
    auto q7 = n.addState();
    auto q8 = n.addState();

    n.addTransition(q0, 'a', q1);
    n.addTransition(q2, 'b', q3);
    n.addTransition(q2, 'b', q3);
    n.addTransition(q8, 'a', q8);
    n.addTransition(q6, 'b', q7);
    n.addTransition(q4, 'a', q3);
    n.addTransition(q3, 'b', q5);

    print(n);

    n.removeUnreachableStates();
    print(n);
}

void testDeterminization() {
    Automata::Alphabet S({'a', 'b', 'c', 'd'});
    NDFA abc = NDFAFactory::exact(S, "abc");
    NDFA abd = NDFAFactory::exact(S, "abd");
    NDFA aacd = NDFAFactory::exact(S, "aacd");

    NDFA n = abc | abd | aacd;
    print(n);

    DFA d = n.determinized();
    print(d);
}

void testConcat() {
    Automata::Alphabet S({'a', 'b'});
    NDFA a = NDFAFactory::exact(S, "a");
    NDFA b = NDFAFactory::exact(S, "b");

    NDFA ab = a * b;
    print(ab);
}

void testEmptyWord() {
    NDFA n = NDFAFactory::emptyWord();
    std::cout << n.accepts("");
}

void testEmptyLanguage() {
    NDFA emptyL = NDFAFactory::emptyLanguage();
}

void testRev() {
    DFA d = NDFAFactory::prefix({'a', 'b', 'c'}, "abc").determinized();
    print(d);
    NDFA rev = d.reversed();
    print(rev);
    std::cout << rev.accepts("abccba");
}

void testMinimization() {
    NDFA n = NDFAFactory::infix({'a', 'b', 'c'}, "cab");
    DFA min = n.minimized();
    print(min);
}

void testPurvoKontrolno() {
    NDFA L1 = NDFAFactory::exact({'a', 'b'}, "a") |
              NDFAFactory::exact({'a', 'b'}, "ab");
    NDFA L2 = NDFAFactory::exact({'c'}, "c");

    NDFA L = *(*L1 | L1 * (*L2));

    DFA min = L.minimized();
    min.makeTotal();
    print(min);
    std::cout << min.accepts("");

}

void testDeterminization2() {
    NDFA L = NDFAFactory::exact("koce");

    NDFA s = *L;
    print(s);

    DFA d = s.determinized();
    d.makeTotal();

    print(d);
    std::cout << d.accepts("") << std::endl;
}

typedef Regex::Letter Letter;
typedef Regex::EmptyWord EmptyWord;
typedef Regex::Word Word;
typedef Regex::EmptyLanguage EmptyLanguage;


size_t findBalancedBracket(size_t from, const StringView &expr) {
    int count = 1;
    for (size_t c = from + 1; c < expr.size(); ++c) {
        if (expr[c] == '(') {
            count++;
        } else if (expr[c] == ')') {
            count--;
        }

        if (count == 0) {
            return c;
        }
    }
    return String::npos;
}

//void testRegexToSTring() {
//    Regex r(
////            new Concat(new KleeneStar(new Letter('a')),
////                       new Concat(new Letter('b'),
////                                  new KleeneStar(
////                                          new Union(new Letter('a'),
////                                                    new Concat(new Letter('b'),
////                                                               new Concat(new KleeneStar(new Letter('a')),
////                                                                          new Letter('b')
////                                                               )
////                                                    )
////                                          )
////                                  )
////                       )
//
//            )
//    );
//
//    String s = r.toString();
//    std::cout << s;
//}

void testGNFAConvert() {
    DFA d = NDFAFactory::prefix("ab").determinized();
    print(d);
    GNFA g = GNFA::convertFrom(d);
    print(g);
}
//
//void testRegex() {
//    Regex r = Regex(new Concat(new Union(new Letter('a'), new Letter('a')), new KleeneStar(new Letter('a'))));
//    Regex r1 = Regex(
//            new Union(new Union(new KleeneStar(new Letter('a')), new Letter('b')),
//                      new Concat(new Letter('c'), new Letter('a'))));
//
//    Regex r2 = Regex(new EmptyLanguage);
//    Regex r3 = Regex(new KleeneStar(new Concat(new Letter('b'), new Letter('a'))));
//    std::cout << r3.toString() << std::endl;
//
//    NDFA d = r3.toNDFA();
//    std::cout << d.accepts("baka");
//}

void testRegexFromString() {
    Regex r("(a)*b((a+b(a)*b))*");
    std::cout << r.toString();
}

void testComplement() {
    DFA n = NDFAFactory::prefix("ab").determinized();
    DFA a = !n;

    print(a);
}

void testIntersection() {
    DFA a1 = NDFAFactory::prefix("ab").determinized().total();
    print(a1);
    DFA a2 = NDFAFactory::suffix("ba").determinized().total();
    print(a2);

    DFA a = a1 & a2;
    print(a);
    std::cout << a.accepts("abba");
}

void testDFASave() {
    DFA d = NDFAFactory::prefix({'a', 'b', 'c'}, "abc").determinized();
    std::ofstream bin("automata_atatata.dat", std::ios::binary);
    d.saveTo(bin);
    bin.close();
}

void testDFARead() {
    std::ifstream bin("automata_atatata.dat", std::ios::binary);
    DFA a = DFA::readFrom(bin);
    bin.close();
    print(a);

}

//void testNDFARead() {
//    Regex r = Regex(new Concat(new Union(new Letter('a'), new Letter('a')), new KleeneStar(new Letter('a'))));
//    NDFA f = r.toNDFA();
//    print(f);
//
//    std::ofstream bf("ndfaka.dat", std::ios::binary);
//    f.saveTo(bf);
//    bf.close();
//}

void testNDFAWrite() {
    std::ifstream bf("ndfaka.dat", std::ios::binary);
    NDFA n = NDFA::readFrom(bf);
    print(n);
    bf.close();
}

void StringViewTest() {
    String k;
    std::cin >> k;
    StringView v(k);
}

void zad1() {
    Regex
            r(new Regex::Concat(new Regex::Letter('b'),
                                new Regex::Concat(new Regex::KleeneStar(new Regex::Union(new Regex::Letter('a'),
                                                                                         new Regex::Concat(
                                                                                                 new Letter('a'),
                                                                                                 new Letter('b')
                                                                                         )
                                                                        )
                                ), new Letter('a'))
              )
    );

    NDFA n = r.toNDFA();
//    NDFA b = NDFAFactory::exact('b');
//    NDFA a = NDFAFactory::exact('a');
//
//    NDFA ab = a * b;
//    NDFA u = a | ab;
//    NDFA s = *u;
//
//    NDFA c = s * a;
//    print(c.minimized());
//    NDFA f = (*(a | a * b)) * a;
//
//    std::cout << f.accepts("ab") << std::endl;
//    NDFA d = b * *(a | a * b) * a;
//
//    print(c.minimized());
//    print(d.minimized());
    print(n.minimized().total());
}

void testRegexParser() {
    Regex r("b(a+ab)*a");
    DFA a = r.toNDFA().minimized();
    print(a);
}

void purva() {
    Regex L1 = Regex(new Regex::Union(new Regex::Letter('a'),
                                      new Regex::Concat(new Regex::Letter('a'),
                                                        new Regex::Letter('b'))));
    Regex L2 = Regex(new Regex::Letter('c'));

    Regex L = Regex(new Regex::KleeneStar(new Regex::Union(new Regex::KleeneStar(L1.expr->clone()),
                                                           new Regex::Concat(L1.expr->clone(),
                                                                             new Regex::KleeneStar(
                                                                                     L2.expr->clone())))));

    DFA d = L.toNDFA().minimized().total();
    std::cout << L.toString() << std::endl;
    print(d);
}

void purva2() {
    Regex L1 = Regex(new Regex::Union(new Regex::Letter('a'),
                                      new Regex::Concat(new Regex::Letter('b'),
                                                        new Regex::Letter('a'))));
    Regex L2 = Regex(new Regex::Letter('c'));

    Regex L = Regex(new Regex::KleeneStar(new Regex::Union(new Regex::KleeneStar(L1.expr->clone()),
                                                           new Regex::Concat(new Regex::KleeneStar(L2.expr->clone()),
                                                                             L1.expr->clone()))));
    DFA d = L.toNDFA().minimized().total();
    std::cout << L.toString() << std::endl;
    print(d);
}

void purva3() {
    Regex L2 = Regex(new Regex::Union(new Regex::Letter('b'),
                                      new Regex::Concat(new Regex::Letter('b'),
                                                        new Regex::Letter('c'))));
    Regex L1 = Regex(new Regex::Letter('a'));

    Regex L = Regex(new Regex::KleeneStar(new Regex::Union(new Regex::KleeneStar(L2.expr->clone()),
                                                           new Regex::Concat(new Regex::KleeneStar(L1.expr->clone()),
                                                                             L2.expr->clone()))));
    DFA d = L.toNDFA().minimized().total();
    std::cout << L.toString() << std::endl;
    print(d);
}

void purva4() {
    Regex L2 = Regex(new Regex::Union(new Regex::Letter('b'),
                                      new Regex::Concat(new Regex::Letter('c'),
                                                        new Regex::Letter('b'))));
    Regex L1 = Regex(new Regex::Letter('a'));

    Regex L = Regex(new Regex::KleeneStar(new Regex::Union(new Regex::KleeneStar(L2.expr->clone()),
                                                           new Regex::Concat(L2.expr->clone(),
                                                                             new Regex::KleeneStar(
                                                                                     L1.expr->clone())))));
    DFA d = L.toNDFA().minimized().total();
    std::cout << L.toString() << std::endl;
    print(d);
}

void testDFAToRegex() {
    Regex r(new Regex::Concat(new Letter('a'),
                              new Regex::Concat(
                                      new Regex::KleeneStar(new Regex::Concat(new Letter('a'), new Letter('b'))),
                                      new Letter('a'))));
    DFA d = r.toNDFA().minimized();
    GNFA g = GNFA::convertFrom(d);
    std::cout << g.convertToRegex();
}

void testDFARemove() {
    DFA d = NDFAFactory::exact("abba").determinized();
    print(d);
    d.removeState(4);
    print(d);

    GNFA g = GNFA::convertFrom(d);
    print(g);
    //g.removeState(4);
}

void testRegex() {
    Regex r("(((a+b))*+ab)");

    Regex s("((a+b))*");
    NDFA S = s.toNDFA();

    Regex ab("ab");
    NDFA AB = ab.toNDFA();

    NDFA N = S | AB;
    print(N);
    std::cout << r.toString() << std::endl;
    NDFA n = r.toNDFA();
    print(n);
}