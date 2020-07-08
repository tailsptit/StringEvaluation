//
// Created by tails on 08/07/2020.
//

#include <cctype>
#include <sstream>
#include <iostream>

#include "../include/Math.h"

bool Math::isOperator(char c) {
    return (c == '+') || (c == '-') || (c == '*') || (c == '/');
}

int Math::getWeight(char op) {
    if ((op == '+') || (op == '-'))
        return 1;
    else if ((op == '*') || (op == '/'))
        return 2;
    return 0;
}

double Math::applyOp(char op, double a, double b){
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':return a / b;
    }
    return 0;
}

bool Math::hasPriority(char op1, char op2){
    if (op2  == '(' || op2 == ')')
        return false;
    if ((op1  == '*' || op1 == '/') && (op2  == '+' || op2 == '-'))
        return false;
    else
        return true;
}
///*
//  Infix to postfix conversion. Only +, -, *, / operators are expected.
//*/
//std::string Math::postfixConversion(std::string exp) {
//    std::string postfix;
//    Stack<char> stack;     //stack to convert string from infix to postfix
//    // scan all characters one by one
//    for (int i = 0; i < exp.length(); ++i) {
//        char c = exp.at(i);
//        // if character is blank space then continue
//        if (isblank(c)) continue;
//
//        if (isdigit(c) || isalpha(c)){
//            postfix += c;
//            if ((i+1 < exp.length()) && (!isdigit(exp.at(i+1)))){
//                postfix += ' ';
//            }
//        }
//        // if the character is an '(' then push it to stack
//        else if (c == '('){
//            stack.push(c);
//        }
//        // if the character is an ')', pop and output from the stack util an '(' is encountered
//        else if (c == ')'){
//            while (!stack.empty() && stack.peek() != '(')
//                postfix += stack.pop();
//            if (!stack.empty() && stack.peek() != '(')
//                return "Invalid Expression";
//            else
//                stack.pop();
//        } else {
//            while (!stack.empty() && getWeight(c) <= getWeight(stack.peek())){
//                if (stack.peek() == '(')
//                    return "Invalid Expression";
//                postfix += stack.pop();
//            }
//            stack.push(c);
//        }
//    }
//    // pop all the operators from the stack
//    while (!stack.empty()) {
//        if (stack.peek() == '(')
//            return "Invalid Expression";
//        postfix += stack.pop();
//    }
//    return postfix;
//}
//
//// evaluatePostfix Postfix exp and return output
//double Math::evaluatePostfix(std::string exp) {
//    Stack<double> stack;
//    // scan all characters one by one
//    for (int i = 0; i < exp.length(); i++) {
//        // if character is blank space then continue
//        if (exp[i] == ' ') continue;
//        else if (isOperator(exp[i])) {
//            // pop two operands from stack.
//            double operand2 = stack.peek(); stack.pop();
//            double operand1 = stack.peek(); stack.pop();
//            switch (exp[i]) {
//                case '+': stack.push(operand1 + operand2); break;
//                case '-': stack.push(operand1 - operand2); break;
//                case '*': stack.push(operand1 * operand2); break;
//                case '/': stack.push(operand1 / operand2); break;
//                default: break;
//            }
//        } else if (isdigit(exp[i])) {
//            // Extract the operand from the string, keep incrementing i as long as getting a digit
//            double operand = 0.0;
//            while (i < exp.length() && isdigit(exp[i])) {
//                // For a number with more than one digits, as we are scanning from left to right
//                operand = (10.0 * operand) + (double)(exp[i] - '0');
//                i++;
//            }
//            // when of while loop with i set to a non-numeric character or end of string
//            // decrement i because it will be incremented in increment section of loop once again.
//            i--;
//            stack.push(operand);
//        }
//    }
//    return stack.peek();
//}

double Math::evaluate(const std::string& exp) {
    Stack<double> values;
    Stack<char> ops;
    int i;

    // scan all characters one by one
    for (i = 0; i < exp.length(); i++) {
        // if character is blank space then continue
        if (isblank(exp[i]) ) continue;

        if (exp[i] == '('){
            ops.push(exp[i]);
        }
        else if (isdigit(exp[i])){
            double digit = 0.0;
            while (i < exp.length() && isdigit(exp[i])) {
                digit = (digit * 10.0) + (exp[i] - '0');
                i++;
            }
            i--;
            values.push(digit);
        }
        // if the character is an ')', pop and output from the stack util an '(' is encountered
        else if (exp[i] == ')'){
            while(!ops.empty() && ops.peek() != '('){
                int val2 = values.pop();
                int val1 = values.pop();
                values.push(applyOp(ops.pop(), val1, val2));
            }
            // pop opening brace.
            if(!ops.empty())
                ops.pop();
        }
        // character is an operator
        else  if (isOperator(exp[i])){
            while (!ops.empty() && getWeight(ops.peek()) >= getWeight(exp[i])){
                int val2 = values.pop();
                int val1 = values.pop();
                values.push(applyOp(ops.pop(), val1, val2));
            }
            // put current value to ops
            ops.push(exp[i]);
        }
    }
    // pop all the operators from the stack
    while (!ops.empty()){
        int val2 = values.pop();
        int val1 = values.pop();
        values.push(applyOp(ops.pop(), val1, val2));
    }
    return values.pop();
}
