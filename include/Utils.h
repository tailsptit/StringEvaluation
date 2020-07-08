//
// Created by tails on 08/07/2020.
//

#ifndef STRINGEVALUATION_UTILS_H
#define STRINGEVALUATION_UTILS_H

class Utils {
public:
    template<typename T>
    static T max(T t1, T t2) { return t1 > t2 ? t1 : t2; }

    template<typename T>
    static T min(T t1, T t2) { return t1 < t2 ? t1 : t2; }

};

#endif /* STRINGEVALUATION_UTILS_H */