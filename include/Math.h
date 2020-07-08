//
// Created by tails on 08/07/2020.
//

#ifndef COCCOC_MATH_H
#define COCCOC_MATH_H

#include<string>
#include<stack>

using namespace std;

class Math {
public:
    stack<char> stackC;     //stack to convert string from infix to postfix
    stack<double> stackE;   //stack to calculate postfix

public:
    Math(){};
    static bool isOperator(char c);
    static int getWeight(char op) ;
    string postfixConversion(string exp);
    double evaluatePostfix(string expression);
    double evaluate(string exp);
};

#endif //COCCOC_MATH_H
