#include <iostream>
#include <tuple>
#include <fstream>
#include "CSVParser.h"
#include "printTuple.h"

using namespace std;

int main()
{
   /* ifstream file1("test2.csv");
    CSVParser<int, float, double, std::string> parser1(file1, 0 , ';', '\n', '"');
    for (tuple<int, float, double, std::string> rs : parser1) {
        cout << rs<< endl;
    }*/

    /*cout << endl << "Next: " << endl;
    ifstream file2("test.csv");
    CSVParser<int, float, double, std::string> parser2(file2, 0 , ',', '\n', '"');
    for (tuple<int, float, double, std::string> rs : parser2) {
        cout << rs<< endl;
    }*/

    cout << endl << "Next: " << endl;
    ifstream file3("test3.csv");
    CSVParser<int, float, double, std::string> parser3(file3, 0 , ';', '\n', '/');
    for (tuple<int, float, double, std::string> rs : parser3) {
        cout << rs<< endl;
    }

   /* cout << endl << "Next: " << endl;
    ifstream file4("test4.csv");
    CSVParser<int, float, double, std::string> parser4(file4, 0 , ';', '\n', '/');
    for (tuple<int, float, double, std::string> rs : parser4) {
        cout << rs<< endl;
    }*/

}

