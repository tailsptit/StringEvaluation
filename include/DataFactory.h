//
// Created by tails on 08/07/2020.
//

#ifndef STRINGEVALUATION_DATAFACTORY_H
#define STRINGEVALUATION_DATAFACTORY_H

#include <string>

class DataFactory {
public:
    DataFactory(){};
    static char getOperator(int val);
    void generate(const std::string &file, int numOperands);
    void generate(const std::string &file, int maxOperands, int numExpressions, bool allowBracket);

private:
};

#endif //STRINGEVALUATION_DATAFACTORY_H
