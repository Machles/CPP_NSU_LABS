#include "Map.h"

void Map::createEmptyMap() {
    char** mapHolder = new char* [this->mapSizeX];
    for (size_t i = 0; i < this->mapSizeX; ++i) {
        mapHolder[i] = new char[this->mapSizeY];
    }

    for (size_t i = 0; i < this->mapSizeY; ++i) {
        for (size_t j = 0; j < this->mapSizeX; ++j) {
            mapHolder[i][j] = static_cast<int>(CELL::Cell_Unknown);
        }
    }

    this->map = mapHolder;
}

Map::Map() {
    this->mapSizeX = 17;
    this->mapSizeY = 17;
    this->appleAmount = 0;
    this->bombAmount = 0;
    this->rockAmount = 0;
    this->visibleGlobalMapX = 8;
    this->visibleGlobalMapY = 8;
    this->map = nullptr;
}

Map::Map(const size_t& xSize, const size_t& ySize) {
    this->mapSizeX = xSize;
    this->mapSizeY = xSize;
    this->appleAmount = 0;
    this->bombAmount = 0;
    this->rockAmount = 0;
    this->visibleGlobalMapX = (xSize >= 15) ? 15 : xSize % 15;
    this->visibleGlobalMapY = (ySize >= 15) ? 15 : ySize % 15;

    createEmptyMap();
}

void Map::createRandomGlobalMap() {
    srand(time(nullptr));

    createEmptyMap();

    for (size_t i = 0; i < this->mapSizeY; ++i) {
        for (size_t j = 0; j < this->mapSizeX; ++j) {
            this->map[i][j] = static_cast<int>(CELL::Cell_Empty);
        }
    }

    this->appleAmount = 20;
    this->bombAmount = 10;
    this->rockAmount = 10;

    size_t currentAppleAmount = 0;
    size_t currentBombAmount = 0;
    size_t currentRockAmount = 0;

    for (int i = 0; i < max(this->appleAmount, max(this->rockAmount, this->bombAmount)); ++i) {
        if (currentRockAmount < this->rockAmount) {
            map[rand() % 15][rand() % 15] = static_cast<int>(CELL::Cell_Rock);
            currentRockAmount++;
        }
        if (currentAppleAmount < this->appleAmount) {
            map[rand() % 15][rand() % 15] = static_cast<int>(CELL::Cell_Apple);
            currentAppleAmount++;
        }
        if (currentBombAmount < this->bombAmount) {
            map[rand() % 15][rand() % 15] = static_cast<int>(CELL::Cell_Bomb);
            currentBombAmount++;
        }
    }
}

CELL Map::getCell(const Coordinates& coordinates) {
    // Static Cast can be useful in this case
    switch (map[coordinates.y][coordinates.x]) {
        case static_cast<int>(CELL::Cell_Empty): return CELL::Cell_Empty;
        case static_cast<int>(CELL::Cell_Apple): return CELL::Cell_Apple;
        case static_cast<int>(CELL::Cell_Rock): return CELL::Cell_Rock;
        case static_cast<int>(CELL::Cell_Bomb): return CELL::Cell_Bomb;
        case static_cast<int>(CELL::Cell_Explorer): return CELL::Cell_Explorer;
        case static_cast<int>(CELL::Cell_Sapper): return CELL::Cell_Sapper;
        case static_cast<int>(CELL::Cell_Unknown): return CELL::Cell_Unknown;
    }
}

void Map::setCell(const Coordinates& coordinates, const CELL& cell) {
    map[coordinates.y][coordinates.x] = static_cast<int>(cell);
}

bool Map::isCoordinatesOutOfMap(const Coordinates& coordinates) const {
    if (coordinates.x >= mapSizeX ||
        coordinates.y >= mapSizeY ||
        coordinates.x < 0 ||
        coordinates.y < 0) {
        return true;
    }
    return false;
}

Map::~Map() {
    for (size_t i = 0; i < mapSizeX; ++i) {
        delete[] map[i];
    }
    delete map;
}