//
// Created by tails on 08/07/2020.
//

#ifndef COCCOC_DATAFACTORY_H
#define COCCOC_DATAFACTORY_H

#include <string>

class DataFactory {
public:
    DataFactory(){}
    void generate(const std::string& path, const std::string& file, int n, int range);
    void generate(const std::string &path, const std::string &file, int n, int range, bool brice);
    void save();

    static char getOperator(int val);

private:
};

#endif //COCCOC_DATAFACTORY_H
