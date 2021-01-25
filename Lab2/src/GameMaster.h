#ifndef LAB2_GAMEMASTER_H
#define LAB2_GAMEMASTER_H
#include <iostream>
#include <sstream>
#include "Console.h"
#define AXIS_SIZE 10

using namespace std;

class GameMaster {
private:
    bool ** currentField;
    bool ** lastField;

    bool stepBackFlag = true;
    bool isGameOver = false;

    size_t aliveCells;
    size_t stepCounter;
public:
    GameMaster();
    virtual ~GameMaster();
    bool ** allocateMemoryForField(const size_t & fieldSize);
    bool ** getLastField(){ return lastField; };
    bool ** getCurrentField(){ return currentField; };

    void changeStepBackFlagStatus(const bool & status){ stepBackFlag = status; }

    size_t numCycle(const int & coordinate) const;
    size_t countNeighbours(const size_t & x, const size_t & y, const bool & fieldType) const;

    friend ostream & operator<< (std::ostream & out, GameMaster & GM);

    void playGame(std::istream & stream);
    void resetGame();
    void step(const size_t & n);
    void back();
    void clearCell(const size_t & x, const size_t & y);
    void setCell(const size_t & x, const size_t & y);
};


#endif //LAB2_GAMEMASTER_H
