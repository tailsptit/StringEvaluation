//
// Created by tails on 08/07/2020.
//

#ifndef COCCOC_EXPRESSIONGENERATOR_H
#define COCCOC_EXPRESSIONGENERATOR_H

#include <typeinfo>
#include <iostream>
#include <sstream>
#include <random>

#include "Expression.h"

class Expression {
public:
    Expression(int min, int max, int len){
        minNum = min;
        maxNum = max;
        length = len;

        gen = std::mt19937(rd());
        rnd = std::uniform_int_distribution<>(minNum, maxNum);
    }
    std::string generate();
    int nextInt();
    bool nextBool();
    static char getOperator(int op);
private:
    int length = 10;
    int minNum = 1;
    int maxNum = 100;
    bool allowBrackets = true;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> rnd;
};


#endif //COCCOC_EXPRESSIONGENERATOR_H
