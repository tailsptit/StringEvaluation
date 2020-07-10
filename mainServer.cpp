//
// Created by tails on 04/07/2020.
//
#include <string>
#include <iostream>
#include <Opt.h>

#include "include/ThreadPool.h"
#include "include/CallBack.h"
#include "include/Evaluation.h"
#include "include/TcpServer.h"
#include "include/DataFactory.h"

using namespace std;

int main(int argc, char **argv) {
    Opt opt(argc, argv);

    if (opt.genData) {
        std::cout << "Generating data" << std::endl;
        DataFactory factory;
        if (opt.file.empty()){
            std::string result = factory.generate(opt.numExpressions, opt.numOperands, opt.maxOperand, opt.minOperand, opt.allowBracket, opt.ops);
            std::cout << "GEN EXPRESSION\n" << result << std::endl;
        } else {
            factory.generate(opt.file, opt.numExpressions, opt.numOperands, opt.maxOperand, opt.minOperand, opt.allowBracket, opt.ops);
        }
    } else  {
        std::cout << "Start server" << std::endl;
        TcpServer server(opt.port, opt.poolSize);
        server.start();
    }
}