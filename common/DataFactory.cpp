//
// Created by tails on 08/07/2020.
//
#include<stack>
#include <fstream>
#include <iostream>
#include<string>
#include <random>
#include <iostream>

#include "../include/DataFactory.h"

using namespace std;

void DataFactory::generate(const string &path, const string &file, int n, int range) {
    ofstream output_file(path + file);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distOperand(1, range);
    std::uniform_int_distribution<> distOperator(1, 4);

    output_file << distOperand(gen);
    for (int i = 2; i <= n; i++) {
        output_file << getOperator(distOperator(gen));
        output_file << distOperand(gen);
    }
    output_file << "\n";
    output_file.close();
}

void DataFactory::generate(const string &path, const string &file, int n, int range, bool brice) {
    ofstream output_file(path + file);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distOperand(1, range);
    std::uniform_int_distribution<> distOperator(1, 4);

    output_file << distOperand(gen);
    for (int i = 2; i <= n; i++) {
        output_file << getOperator(distOperator(gen));
        output_file << distOperand(gen);
    }
    output_file << "\n";
    output_file.close();
}

char DataFactory::getOperator(int val){
    switch (val) {
        case 1: return '+';
        case 2: return '-';
        case 3: return '*';
        case 4: return '/';
        default: return '+';
    }
}
