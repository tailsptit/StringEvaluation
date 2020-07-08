//
// Created by tails on 08/07/2020.
//

#include <cctype>

#include "../include/Math.h"

bool Math::isOperator(char c) {
    return (c == '+') || (c == '-') || (c == '*') || (c == '/');
}

int Math::getWeight(char op) {
    if ((op == '+') || (op == '-'))
        return 1;
    else if ((op == '*') || (op == '/'))
        return 2;
    else if (op == '^')
        return 3;
    return -1;
}

/*
  Infix to postfix conversion. Only +, -, *, / operators are expected.
*/
//string Math::postfixConversion(string exp) {
//    string postfix;
//    stack<char> stackC;     //stack to convert string from infix to postfix
//    // scan all characters one by one
//    for (int i = 0; i < exp.length(); i++) {
//        // if character is blank space then continue
//        if (exp[i] == ' ' || exp[i] == ',') continue;
//        //If character is operator, pop two elements from stack, perform operation and push the result back.
//        else if (isOperator(exp[i])) {
//            while (!stackC.empty() && (stackC.top() != '(') &&
//                       (getOpWeight(exp[i]) < getWeight(stackC.top()))) {
//                postfix += stackC.top();
//                stackC.pop();
//            }
//            stackC.push(exp[i]);
//        }
//        // Else if character is an operand
//        else if (isOperand(exp[i])) {
//            postfix += exp[i];
//            if ((i+1 < exp.length()) && (!isOperand(exp[i+1])))
//                postfix += ' ';
//        } else if (exp[i] == '(') {
//            stackC.push(exp[i]);
//        } else if (exp[i] == ')') {
//            while (!stackC.empty() && stackC.top() != '(') {
//                postfix += stackC.top();
//                stackC.pop();
//            }
//            if (!stackC.empty() && stackC.top()){
//                return "Invalid Expression"; // invalid exp
//            } else
//                stackC.pop();
//        }
//    }
//    // pop all the operators from the stack
//    while (!stackC.empty()) {
//        if (stackC.top() == '(')
//            return "Invalid Expression";
//        postfix += stackC.top();
//        stackC.pop();
//    }
//    return postfix;
//}

string Math::postfixConversion(string exp) {
    string postfix;
//    stack<char> stackC;     //stack to convert string from infix to postfix
    // scan all characters one by one
    for (int i = 0; i < exp.length(); ++i) {
        char c = exp.at(i);
        // if character is blank space then continue
        if (isblank(c)) continue;

        if (isdigit(c) || isalpha(c)){
            postfix += c;
            if ((i+1 < exp.length()) && (!isdigit(exp.at(i+1)))){
                postfix += ' ';
            }
        }
        // if the character is an '(' then push it to stack
        else if (c == '('){
            stackC.push(c);
        }
        // if the character is an ')', pop and output from the stack util an '(' is encountered
        else if (c == ')'){
            while (!stackC.empty() && stackC.top() != '('){
                postfix += stackC.top();
                stackC.pop();
            }
            if (!stackC.empty() && stackC.top() != '(')
                return "Invalid Expression";
            else
                stackC.pop();
        } else {
            while (!stackC.empty() && getWeight(c) <= getWeight(stackC.top())){
                if (stackC.top() == '(')
                    return "Invalid Expression";
                postfix += stackC.top();
                stackC.pop();
            }
            stackC.push(c);
        }
    }
    // pop all the operators from the stack
    while (!stackC.empty()) {
        if (stackC.top() == '(')
            return "Invalid Expression";
        postfix += stackC.top();
        stackC.pop();
    }
    return postfix;
}

// evaluatePostfix Postfix expression and return output
double Math::evaluatePostfix(string expression) {
    // scan all characters one by one
    for (int i = 0; i < expression.length(); i++) {
        // if character is blank space then continue
        if (expression[i] == ' ') continue;
        else if (isOperator(expression[i])) {
            // pop two operands from stackE.
            double operand2 = stackE.top(); stackE.pop();
            double operand1 = stackE.top(); stackE.pop();
            switch (expression[i]) {
                case '+': stackE.push(operand1 + operand2); break;
                case '-': stackE.push(operand1 - operand2); break;
                case '*': stackE.push(operand1 * operand2); break;
                case '/': stackE.push(operand1 / operand2); break;
                default: break;
            }
        } else if (isdigit(expression[i])) {
            // Extract the operand from the string, keep incrementing i as long as getting a digit
            double operand = 0.0;
            while (i < expression.length() && isdigit(expression[i])) {
                // For a number with more than one digits, as we are scanning from left to right
                operand = (10.0 * operand) + (double)(expression[i] - '0');
                i++;
            }
            // when of while loop with i set to a non-numeric character or end of string
            // decrement i because it will be incremented in increment section of loop once again.
            i--;
            stackE.push(operand);
        }
    }
    return stackE.top();
}


double Math::evaluate(string exp) {
    string postfix;
//    stack<char> stackC;     //stack to convert string from infix to postfix
    // scan all characters one by one
    for (int i = 0; i < exp.length(); ++i) {
        char c = exp.at(i);
        // if character is blank space then continue
        if (isblank(c)) continue;

        if (isdigit(c) || isalpha(c)){
            postfix += c;
            if ((i+1 < exp.length()) && (!isdigit(exp.at(i+1)))){
                postfix += ' ';
            }
        }
            // if the character is an '(' then push it to stack
        else if (c == '('){
            stackC.push(c);
        }
            // if the character is an ')', pop and output from the stack util an '(' is encountered
        else if (c == ')'){
            while (!stackC.empty() && stackC.top() != '('){
                postfix += stackC.top();
                stackC.pop();
            }
            if (!stackC.empty() && stackC.top() != '(')
                return "Invalid Expression";
            else
                stackC.pop();
        } else {
            while (!stackC.empty() && getWeight(c) <= getWeight(stackC.top())){
                if (stackC.top() == '(')
                    return "Invalid Expression";
                postfix += stackC.top();
                stackC.pop();
            }
            stackC.push(c);
        }
    }
    // pop all the operators from the stack
    while (!stackC.empty()) {
        if (stackC.top() == '(')
            return "Invalid Expression";
        postfix += stackC.top();
        stackC.pop();
    }
    return postfix;
}