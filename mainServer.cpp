//
// Created by tails on 04/07/2020.
//
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <vector>
#include <chrono>
#include <exception>
#include <map>

#include "include/ThreadPool.h"
#include "include/CallBack.h"

#include "include/Math.h"
#include "include/TcpServer.h"
#include "include/TcpClient.h"
#include "include/CallBack.h"
#include "include/DataFactory.h"

using namespace std;

class Opt{
public:
    Opt(int argc, char **argv){
        
    }
};

int main(int argc, char **argv) {
    Opt opt(argc, argv);

    int port = 8081;
    if (argc > 1) {
        port = std::stoi(std::string(argv[1]));
    }
    TcpServer server(port, 5);
    server.start();

//    DataFactory dataFactory;
//    dataFactory.generate("/home/tails/Documents/Project/C++/", "test0.txt", 10, 100);

//    TcpClient client("localhost", port);
//
//
//    // for (int i = 0; i < 10000; i++) {
//    //   if (client.RunOneSession() != 0) {
//    //     fprintf(stderr, "FAILED\n");
//    //     return -1;
//    //   }
//    //   std::cout << "Success :) " << i << std::endl;
//    // }
//    for (int i = 0; i < 500; i++) {
//        client.runConcurrentSessions(1000);
//    }

//    ThreadPool pool(4);
//    std::vector< std::future<int> > results;
//    pool.start();
//    pool.addTask(getRefCallBack(func, 9));
//    for(int i = 0; i < 10000; ++i) {
//        pool.addTask(new CallBack([i] {
//            std::cout << "hello " << i << std::endl;
//            //std::this_thread::sleep_for(std::chrono::seconds(1));
//            std::cout << "world " << i << std::endl;
//            return i*i;
//        }));
//    }
//
//    pool.stop();
//    pool.awaitTermination();
//    std::cout << "All pQueueTasks complted." << std::endl;


//    int i = 10;
//    long l = 10;
//    long long ll = 10;
//    float f = 1.2;
//    double d = 2.2;
//    cout << "int getSize       : " << sizeof(i) << endl;
//    cout << "long getSize      : " << sizeof(l) << endl;
//    cout << "long long getSize : " << sizeof(ll) << endl;
//    cout << "float getSize: " << sizeof(f) << endl;
//    cout << "double getSize: " << sizeof(d) << endl;

//    string factory;
//    Math math;
//    math.generateData();
//    string datafile = "test0";
//    ifstream ifile;
//    ofstream ofile;
//    string input_file = "/home/tails/Documents/Project/C++/" + datafile + ".txt";
//    string output_file = "/home/tails/Documents/Project/C++/" + datafile + "_out.txt";
//
//    ifile.open(input_file);
//    getline(ifile, factory);
////    cout << "Input: " << factory << "\n";
//    string postfix = math.postfixConversion(factory);
//    ifile.closeFd();
//    cout << "postfixConversion completed: \n";
//    factory.clear();
//    ofile.open(output_file);
//    ofile << "Output:" << postfix << "\n";
//    ofile.closeFd();
//
//    ifile.open(output_file);
////    getline(ifile, factory);
//    int result = math.evaluatePostfix(postfix);
//    cout << "Output = " << result << "\n";
//    ofile.closeFd();
}

//int main() {
//    string factory;
//    Math math;
//    math.generateData();
//    string datafile = "test0";
//    ifstream ifile;
//    ofstream ofile;
//    string input_file = "/home/tails/Documents/Project/C++/" + datafile + ".txt";
//    string output_file = "/home/tails/Documents/Project/C++/" + datafile + "_out.txt";
//    ifile.open(input_file);
//    char input[7] = "";
//    string str;
//    int length = 7;             // getNumEvents of chars you want to read
//    str.resize(length, ' ');     // reserve spaces
//    char *begin = &*str.begin();
//
////    ifile.seekg(0, std::ifstream::end);
////    int len = ifile.tellg();
////    ifile.seekg(0, std::ifstream::beg);
////    cout << "LEN = " << len << endl;
//
//    while (!ifile.eof()) {
////        ifile.read(input, sizeof(input) - 1);
////        cout << "=" << sizeof(input) << endl;
//        ifile.read(input, length);
//        cout << "getSize = " <<  ":" << input << endl;
////        cout << begin << endl;
//
//    }
//    ifile.closeFd();
////
////
////    getline(ifile, factory);
//////    cout << "Input: " << factory << "\n";
////    string postfix = math.postfixConversion(factory);
////    ifile.closeFd();
////
////    ofile.open(output_file);
////    ofile << "Output:" << postfix << "\n";
////    ofile.closeFd();
////
////    ifile.open(output_file);
////    getline(ifile, factory);
////    int result = math.evaluatePostfix(factory);
////    cout << "Output = " << result << "\n";
////    ofile.closeFd();
//}