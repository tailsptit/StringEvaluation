//
// Created by tails on 04/07/2020.
//
#include <string>
#include <iostream>

#include "include/ThreadPool.h"
#include "include/CallBack.h"
#include "include/Evaluation.h"
#include "include/TcpServer.h"
#include "include/DataFactory.h"

using namespace std;

class Opt {
public:
    Opt(int argc, char **argv) {
        if (argc > 1) {
            for (int i = 1; i < argc; i = i + 2) {
                std::string tag(argv[i]);
                if (tag.compare("-run-mode") == 0) {
                    run_mode = std::stoi(std::string(argv[i + 1]));
                } else if (tag.compare("-port") == 0) {
                    port = std::stoi(std::string(argv[i + 1]));
                } else if (tag.compare("-file") == 0) {
                    string val(argv[i + 1]);
                    file = val;
                } else if (tag.compare("-pool-size") == 0) {
                    poolSize = std::stoi(std::string(argv[i + 1]));
                } else if (tag.compare("-num-expression") == 0) {
                    numExpressions = std::stoi(std::string(argv[i + 1]));
                } else if (tag.compare("-max-num-operand") == 0) {
                    maxOperands = std::stoi(std::string(argv[i + 1]));
                }
            }
        }
    }

public:
    int port = 8081;
    int run_mode = 2; //default start server
    int poolSize = 7;
    string file = "";
    int maxOperands = 10;
    int numExpressions = 1;
    bool allowBracket = true;
};

int main(int argc, char **argv) {
    std::cout << "argc = " << argc << std::endl;
    std::cout << "**argv = " << **argv << std::endl;
    Opt opt(argc, argv);

    if (opt.run_mode == 1) {
        std::cout << "Generating data. No start server" << std::endl;
        DataFactory factory;
        if (opt.file.compare("") == 0){
            std::string result = factory.generate(opt.maxOperands, opt.numExpressions, opt.allowBracket);
            std::cout << "OUTPUT\n" << result << std::endl;
        } else {
            factory.generate(opt.file, opt.maxOperands, opt.numExpressions, opt.allowBracket);
        }

    } else if (opt.run_mode == 2) {
        std::cout << "Start server" << std::endl;
        TcpServer server(opt.port, opt.poolSize);
        server.start();
    } else if (opt.run_mode == 3) {
        std::cout << "Generating data & Start server" << std::endl;
        DataFactory dataFactory;
        dataFactory.generate(opt.file, opt.maxOperands, opt.numExpressions, opt.allowBracket);
        TcpServer server(opt.port, opt.poolSize);
        server.start();
    } else {
        std::cout << "wrong mode" << std::endl;
    }
}