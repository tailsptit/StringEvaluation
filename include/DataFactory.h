//
// Created by tails on 08/07/2020.
//

#ifndef STRINGEVALUATION_DATAFACTORY_H
#define STRINGEVALUATION_DATAFACTORY_H

#include <string>

class DataFactory {
public:
    DataFactory(){};
    char getOperator(int val);
    void generate(const std::string &file, int numOperands);
    std::string generate(int numExpressions, int numOperands, int maxOperand, int minOperand, bool allowBracket, char ops[]);
    void generate(const std::string &file, int numExpressions, int numOperands, int maxOperand, int minOperand, bool allowBracket, char ops[]);


private:
};

#endif //STRINGEVALUATION_DATAFACTORY_H
