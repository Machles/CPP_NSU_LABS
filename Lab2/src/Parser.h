#ifndef LAB2_PARSER_H
#define LAB2_PARSER_H

#define AXIS_SIZE 10

#include <fstream>
#include <iostream>
#include "GameMaster.h"

using namespace std;
class Parser {
public:
    void loadGameFromFile(GameMaster * GM, const string & filename);
    void saveGameIntoFile(bool ** field, const string & filename);
};


#endif //LAB2_PARSER_H
