//
// Created by tails on 08/07/2020.
//
#include <string>
#include <fstream>

#include "../../include/Expression.h"

std::string Expression::generate(int numExpression) {
    std::stringstream buffer;
    for (int expId = 0; expId < numExpression; expId++) {
        // generates and appends the first number to file
        buffer << nextInt();
        int brackets = 0;
        for (int i = 0; i < numOperands; i++) {
            int number = nextInt();
            // Generates random operator and append
            char op = ops[nextInt() % 4];
            if (op == '/' && number == 0) {
                i--;
                continue;
            }
            buffer << op;

            // Creates a bracket
            bool inBrackets = false;
            if (nextBool() && i != numOperands - 1 && allowBrackets) {
                buffer << "(";
                inBrackets = true;
                brackets++;
            }
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
    return buffer.str();
}

void Expression::generate(const std::string& file, int numExpression) {
    std::ofstream buffer(file);
    for (int expId = 0; expId < numExpression; expId++) {
        // generates and appends the first number to file
        buffer << nextInt();
        int brackets = 0;
        for (int i = 0; i < numOperands; i++) {
            // Generates random number
            int number = nextInt();

            // Generates random operator and append
            char op = ops[nextInt() % 4];
            if (op == '/' && number == 0) {
                i--;
                continue;
            }
            buffer << op;

            // Creates a bracket
            bool inBrackets = false;
            if (nextBool() && i != numOperands - 1 && allowBrackets) {
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