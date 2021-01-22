#include <iostream>
#include <tuple>
#include <fstream>
#include "CSVParser.h"
#include "printTuple.h"

using namespace std;

int main()
{
    ifstream file("test.csv");
    CSVParser<int, float, double, std::string> parser(file, 0 , ',', '\n', '"'/*skip first lines count*/);
    for (tuple<int, float, double, std::string> rs : parser) {
        cout << rs<< endl;
    }
}

