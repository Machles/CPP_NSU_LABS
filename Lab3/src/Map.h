#ifndef LAB3_MAP_H
#define LAB3_MAP_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include "Cell.h"
#include "Coordinates.h"

class Map {
private:

    size_t mapSizeX;
    size_t mapSizeY;

    size_t appleAmount;
    size_t bombAmount;
    size_t rockAmount;

    char** map;

    size_t visibleGlobalMapX; // Ширина куска карты, отображаемой графически
    size_t visibleGlobalMapY; // Высота куска карты, отображаемой графически
public:
    Map(const size_t& xSize, const size_t& ySize);
    Map();
    ~Map();
    void createEmptyMap();

    void setMapSizeX(const size_t& mapSizeX) { this->mapSizeX = mapSizeX; };
    void setMapSizeY(const size_t& mapSizeY) { this->mapSizeY = mapSizeY; };
    void setAppleAmount(const size_t& appleAmount) { this->appleAmount = appleAmount; };
    void setRockAmount(const size_t& rockAmount) { this->rockAmount = rockAmount; };
    void setBombAmount(const size_t& bombAmount) { this->bombAmount = bombAmount; };
    void setVisibleGlobalMapX(const size_t& visibleGlobalMapX) { this->visibleGlobalMapX = visibleGlobalMapX; };
    void setVisibleGlobalMapY(const size_t& visibleGlobalMapY) { this->visibleGlobalMapY = visibleGlobalMapY; };

    size_t getAppleAmount() { return this->appleAmount; }
    size_t getBombAmount() { return this->bombAmount; }
    
    size_t getVisibleGlobalMapX() { return visibleGlobalMapX; };
    size_t getVisibleGlobalMapY() { return visibleGlobalMapY; };
    size_t getMapSizeX() { return mapSizeX; };
    size_t getMapSizeY() { return mapSizeY; };

    CELL getCell(const Coordinates& coordinates);
    void setCell(const Coordinates& coordinates, const CELL& cell);

    bool isCoordinatesOutOfMap(const Coordinates& coordinates) const;
    
    void setMap(char** map) { this->map = map; };
    char** getMap() { return this->map; };

    void createRandomGlobalMap();

    friend std::ostream& operator<< (std::ostream& out, Map& M);
};

#endif //LAB3_MAP_H