//
// Created by tails on 08/07/2020.
//
#include <string>
#include <fstream>

#include "../../include/Expression.h"

std::string Expression::generate() {
    std::stringstream buffer;

    // Generates and appends the first number
    buffer << nextInt();
    int brackets = 0;
    for (int i = 0; i < length; i++) {
        // Generates random number
        int number = nextInt();

        // Generates random operator and append
        char op = getOperator(nextInt() % 4);
        if (op =='/' && number == 0) {
            i--;
            continue;
        }
        buffer << " " << op << " ";

        // Creates a bracket
        bool inBrackets = false;
        if (nextBool() && i != length - 1 && allowBrackets) {
            buffer << "(";
            inBrackets = true;
            brackets++;
        }

        // Appends the generated number
        buffer << number;

        // Ends a bracket if exists
        if (brackets > 0 && !inBrackets && nextBool()) {
            buffer << ")";
            brackets--;
        }
    }
    for (int i = 0; i < brackets; i++) {
        buffer << ")";
    }
    brackets = 0;
    // Ends all brackets
    return buffer.str();
}

void Expression::generateAndSaveFile(std::string file, int numExpression ) {
    std::ofstream buffer(file);
    for (int n = 0; n < numExpression; n++){
        // Generates and appends the first number
        int len = 1 + nextInt() % maxLen;
        buffer << nextInt();
        int brackets = 0;
        for (int i = 0; i < len; i++) {
            // Generates random number
            int number = nextInt();

            // Generates random operator and append
            char op = getOperator(nextInt() % 4);
            if (op =='/' && number == 0) {
                i--;
                continue;
            }
            buffer << " " << op << " ";

            // Creates a bracket
            bool inBrackets = false;
            if (nextBool() && i != length - 1 && allowBrackets) {
                buffer << "(";
                inBrackets = true;
                brackets++;
            }

            // Appends the generated number
            buffer << number;

            // Ends a bracket if exists
            if (brackets > 0 && !inBrackets && nextBool()) {
                buffer << ")";
                brackets--;
            }
        }
        // Ends all brackets
        for (int i = 0; i < brackets; i++) {
            buffer << ")";
        }
        buffer << '\n';
    }
    buffer.close();
}

int Expression::nextInt() {
    return rnd(gen);
}

bool Expression::nextBool() {
    return rnd(gen) % 2 == 1;
}

char Expression::getOperator(int op){
    switch (op) {
        case 0: return '+';
        case 1: return '-';
        case 2: return '*';
        case 3: return '/';
        default: return '+';
    }
}