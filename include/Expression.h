//
// Created by tails on 08/07/2020.
//

#ifndef STRINGEVALUATION_EXPRESSIONGENERATOR_H
#define STRINGEVALUATION_EXPRESSIONGENERATOR_H

#include <typeinfo>
#include <iostream>
#include <sstream>
#include <random>

#include "Expression.h"

class Expression {
public:
    Expression(int _maxOperands = 100, bool bracket = true){
        maxOperands = _maxOperands;
        allowBrackets = bracket;
        gen = std::mt19937(rd());
        rnd = std::uniform_int_distribution<>(1, 1000);
    }
    std::string generate(int numExpression);
    void generate(std::string file, int numExpression);
    int nextInt();
    bool nextBool();
    static char getOperator(int op);

private:
    int maxOperands;
    bool allowBrackets = true;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> rnd;
};


#endif //STRINGEVALUATION_EXPRESSIONGENERATOR_H
