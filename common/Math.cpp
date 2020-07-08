//
// Created by tails on 08/07/2020.
//

#include <cctype>
#include <sstream>

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

double Math::applyOp(char op, double a, double b){
    switch (op) {
        case '+' : return a + b;
        case '-' : return a - b;
        case '*' : return a * b;
        case '/' :
            return a / b;
    }
}

bool Math::hasPriority(char op1, char op2){
    if (op2  == '(' || op2 == ')')
        return false;
    if ((op1  == '*' || op1 == '/') && (op1  == '*' || op1 == '/'))
        return false;
    else
        return true;
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

std::string Math::postfixConversion(std::string exp) {
    std::string postfix;
    Stack<char> stack;     //stack to convert string from infix to postfix
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
            stack.push(c);
        }
        // if the character is an ')', pop and output from the stack util an '(' is encountered
        else if (c == ')'){
            while (!stack.empty() && stack.top() != '(')
                postfix += stack.pop();
            if (!stack.empty() && stack.top() != '(')
                return "Invalid Expression";
            else
                stack.pop();
        } else {
            while (!stack.empty() && getWeight(c) <= getWeight(stack.top())){
                if (stack.top() == '(')
                    return "Invalid Expression";
                postfix += stack.pop();
            }
            stack.push(c);
        }
    }
    // pop all the operators from the stack
    while (!stack.empty()) {
        if (stack.top() == '(')
            return "Invalid Expression";
        postfix += stack.pop();
    }
    return postfix;
}

// evaluatePostfix Postfix exp and return output
double Math::evaluatePostfix(std::string exp) {
    Stack<double> stack;
    // scan all characters one by one
    for (int i = 0; i < exp.length(); i++) {
        // if character is blank space then continue
        if (exp[i] == ' ') continue;
        else if (isOperator(exp[i])) {
            // pop two operands from stack.
            double operand2 = stack.top(); stack.pop();
            double operand1 = stack.top(); stack.pop();
            switch (exp[i]) {
                case '+': stack.push(operand1 + operand2); break;
                case '-': stack.push(operand1 - operand2); break;
                case '*': stack.push(operand1 * operand2); break;
                case '/': stack.push(operand1 / operand2); break;
                default: break;
            }
        } else if (isdigit(exp[i])) {
            // Extract the operand from the string, keep incrementing i as long as getting a digit
            double operand = 0.0;
            while (i < exp.length() && isdigit(exp[i])) {
                // For a number with more than one digits, as we are scanning from left to right
                operand = (10.0 * operand) + (double)(exp[i] - '0');
                i++;
            }
            // when of while loop with i set to a non-numeric character or end of string
            // decrement i because it will be incremented in increment section of loop once again.
            i--;
            stack.push(operand);
        }
    }
    return stack.top();
}

double Math::evaluate(const std::string& exp) {
    Stack<double> values;
    Stack<char> ops;

    std::string tokens = exp;
//    char tokens[exp.size() + 1];
//    std::copy(exp.begin(), exp.end(), tokens);

    // scan all characters one by one
    for (int i = 0; i < exp.length(); ++i) {
        // if character is blank space then continue
        if (isblank(tokens[i] )) continue;
        if (isdigit(tokens[i])){
            std::stringbuf buf;
            while (i < tokens.length() && isdigit(tokens[i]))
                buf.sputc(tokens[i]);
            values.push(std::stol(buf.str()));
        }
        // if the character is an '(' then push it to stack
        else if (tokens[i] == '('){
            ops.push(tokens[i]);
        }
        // if the character is an ')', pop and output from the stack util an '(' is encountered
        else if (tokens[i] == ')'){
            while (ops.top() != '(')
                values.push(applyOp(ops.pop(), ops.pop(), values.pop()));
            ops.pop();
        }
        // current tokens is an operator
        else  if (isOperator(tokens[i])){
            while (!ops.empty() && hasPriority(tokens[i], ops.top()))
                values.push(applyOp(ops.pop(), ops.pop(), values.pop()));
            // put current token to ops
            ops.push(tokens[i]);
        }
    }
    // pop all the operators from the stack
    while (!ops.empty())
        values.push(applyOp(ops.pop(), ops.pop(), values.pop()));
    return values.pop();
}
