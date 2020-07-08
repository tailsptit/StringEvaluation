//
// Created by tails on 08/07/2020.
//

#ifndef STRINGEVALUATION_EVALUATION_H
#define STRINGEVALUATION_EVALUATION_H

#include<string>
#include<stack>

template<typename T>
class Stack {
public:
    void push(T val){ _stack.push(val); }
    bool empty(){ return _stack.empty(); }
    T peek(){ return _stack.top(); }
    T pop(){
        T val = _stack.top();
        _stack.pop();
        return val;
    }
private:
    std::stack<T> _stack;
};

class Evaluation {
public:
    Evaluation(){};
    static bool isOperator(char c);
    static int getWeight(char op) ;
    static double applyOp(char op, double a, double b);
    static bool hasPriority(char op1, char op2);
//    static std::string postfixConversion(std::string exp) ;
//    static double evaluatePostfix(std::string exp);
    static double evaluate(const std::string& exp);
};

#endif //STRINGEVALUATION_EVALUATION_H
