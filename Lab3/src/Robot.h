#ifndef LAB3_ROBOT_H
#define LAB3_ROBOT_H

#include <stdlib.h>
#include <vector>
#include <unordered_map>
#include "Map.h"
#include "Direction.h"
#include "Coordinates.h"
#include "Environment.h"
#include "Receiver.h"

class Robot {
protected:
    Map* intMap;
    Coordinates position;
    Environment* environment;
    Receiver* receiver;
public:
    Robot();
    virtual ~Robot();

    void setReceiver(Receiver* receiver) { this->receiver = receiver; }
    Receiver* getReceiver() { return this->receiver; }

    Environment* getEnvironment() { return this->environment; }
    void* setEnvironment(Environment* environment) { this->environment = environment; }

    Map* getInternalMap() { return this->intMap; };
    void setInternalMap(Map* map) { this->intMap = map; };

    bool isRobotAbleToStep(const Coordinates& coordinates, Robot * robot) const;
    Coordinates createNewCoordinates(const Direction& direction);
    Coordinates getValidCoordinatesForRobotSpawn();

    void setCoordinates(const Coordinates& coordinates) { this->position = coordinates;}
    const Coordinates& getCoordinates() { return this->position;}

    virtual void move(const Direction& direction) = 0;
    void updateMap();

    size_t getXCoordinate();
    size_t getYCoordinate();
    
    Direction getDirection(const Coordinates& diff);
};

class Explorer : public Robot {
public:
    Explorer(Map * globalMap, Receiver * receiver);
    virtual ~Explorer();

    void move(const Direction& direction) override;
    void grabApple();
    void scanWorldAround();
    void subScan(const int& xAdd, const int& yAdd);
};

class Sapper : public Robot {
public:
    Sapper(Map* globalMap, Receiver* receiver);
    virtual ~Sapper();

    void move(const Direction& direction) override;
    void defuseBomb();
};


#endif //LAB3_ROBOT_H