//
// Created by tails on 08/07/2020.
//
#include<stack>
#include<string>
#include <fstream>

#include "../../include/Expression.h"
#include "../../include/DataFactory.h"

char DataFactory::getOperator(int val){
    switch (val) {
        case 1: return '+';
        case 2: return '-';
        case 3: return '*';
        case 4: return '/';
        default: return '+';
    }
}

void DataFactory::generate(const std::string &file, int numOperands) {
    std::ofstream output_file(file);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distOperand(1, 1000);
    std::uniform_int_distribution<> distOperator(1, 4);
    output_file << distOperand(gen);
    for (int i = 2; i <= numOperands; i++) {
        output_file << getOperator(distOperator(gen));
        output_file << distOperand(gen);
    }
    output_file << "\n";
    output_file.close();
}

std::string DataFactory::generate(int numExpressions, int numOperands, int maxOperand, int minOperand, bool allowBracket, char ops[]) {
    Expression  exp(numOperands, maxOperand, minOperand, allowBracket, ops);
    return exp.generate(numExpressions);
}

void DataFactory::generate(const std::string &file, int numExpressions, int numOperands, int maxOperand, int minOperand, bool allowBracket, char ops[]) {
    Expression  exp(numOperands, maxOperand, minOperand, allowBracket, ops);
    exp.generate(file, numExpressions);
}

