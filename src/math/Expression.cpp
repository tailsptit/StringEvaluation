//
// Created by tails on 08/07/2020.
//
#include <string>
#include <fstream>

#include "../../include/Expression.h"

std::string Expression::generate(int numExpression) {
    std::stringstream buffer;
    for (int n = 0; n < numExpression; n++) {
        int numberOfOperandsInExpression = 1 + nextInt() % maxOperands;

        // generates and appends the first number to file
        buffer << nextInt();
        int brackets = 0;
        for (int i = 0; i < numberOfOperandsInExpression; i++) {
            // Generates random number
            int number = nextInt();

            // Generates random operator and append
            char op = getOperator(nextInt() % 4);
            if (op == '/' && number == 0) {
                i--;
                continue;
            }
            buffer << " " << op << " ";

            // Creates a bracket
            bool inBrackets = false;
            if (nextBool() && i != numberOfOperandsInExpression - 1 && allowBrackets) {
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
//    std::cout << "xxxxxxx = "<< buffer.str() << std:: endl;
    return buffer.str();
}

void Expression::generate(std::string file, int numExpression) {
    std::ofstream buffer(file);
    for (int n = 0; n < numExpression; n++) {
        int numberOfOperandsInExpression = 1 + nextInt() % maxOperands;

        // generates and appends the first number to file
        buffer << nextInt();
        int brackets = 0;
        for (int i = 0; i < numberOfOperandsInExpression; i++) {
            // Generates random number
            int number = nextInt();

            // Generates random operator and append
            char op = getOperator(nextInt() % 4);
            if (op == '/' && number == 0) {
                i--;
                continue;
            }
            buffer << " " << op << " ";

            // Creates a bracket
            bool inBrackets = false;
            if (nextBool() && i != numberOfOperandsInExpression - 1 && allowBrackets) {
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

char Expression::getOperator(int op) {
    switch (op) {
        case 0:
            return '+';
        case 1:
            return '-';
        case 2:
            return '*';
        case 3:
            return '/';
        default:
            return '+';
    }
}