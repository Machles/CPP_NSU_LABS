#ifndef LAB2_CONSOLE_H
#define LAB2_CONSOLE_H

#include <string>
#define AXIS_SIZE 10

using namespace std;

enum COMMAND { RESET, BACK, SET, CLEAR, STEP, LOAD, SAVE };

class Console {
public:
    COMMAND getCommand(const string & command);
    bool isItValidNumber(const string & numberString);
    bool isItCorrectCellCoordinates(const string & coordinates);
    bool isItCorrectFilename(const string & filename);
    size_t getNumberFromString(const string & numberString);
};


#endif //LAB2_CONSOLE_H
