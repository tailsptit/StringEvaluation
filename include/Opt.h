//
// Created by tails on 10/07/2020.
//

#ifndef STRINGEVALUATION_OPT_H
#define STRINGEVALUATION_OPT_H

#include <string>

class Opt {
public:
    Opt(int argc, char **argv) {
        if (argc > 1) {
            for (int i = 1; i < argc; i = i + 2) {
                std::string tag(argv[i]);
                if (tag == "-port") {
                    port = std::stoi(std::string(argv[i + 1]));
                } else if (tag == "-gen-data") {
                    std::string gen = std::string(argv[i + 1]);
                    genData = (gen == "true");
                }  else if (tag == "-file") {
                    std::string val(argv[i + 1]);
                    file = val;
                } else if (tag == "-pool-size") {
                    poolSize = std::stoi(std::string(argv[i + 1]));
                } else if (tag == "-num-expression") {
                    numExpressions = std::stoi(std::string(argv[i + 1]));
                } else if (tag == "-num-operand") {
                    numOperands = std::stoi(std::string(argv[i + 1]));
                } else if (tag == "-max-operand") {
                    maxOperand = std::stoi(std::string(argv[i + 1]));
                } else if (tag == "-min-operand") {
                    minOperand = std::stoi(std::string(argv[i + 1]));
                } else if (tag == "-allow-bracket") {
                    std::string bracket = std::string(argv[i + 1]);
                    allowBracket = (bracket == "true");
                } else if (tag == "-operator") {
                    std::string opsStr = std::string(argv[i + 1]);
                    for (int o = 0; o < 4; o++) {
                        if (o < opsStr.length()){
                            ops[o] = opsStr[o];
                        } else {
                            ops[o] = opsStr[0];
                        }
                    }
                }

            }
        }
    }

public:
    bool genData = false;
    int port = 8081;
    int run_mode = 2; //default start server
    int poolSize = 7;
    std::string file;
    int maxOperand  = 1000;
    int minOperand  = 0;
    int numOperands = 10;
    int numExpressions = 1;
    bool allowBracket = true;
    char ops[4] = {'+', '-', '*', '/'}; // default
};
#endif //STRINGEVALUATION_OPT_H
