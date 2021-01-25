#ifndef LAB3_ENVIRONMENT_H
#define LAB3_ENVIRONMENT_H

#include "Cell.h"
#include "Coordinates.h"

#include <cstdlib>
#include <vector>

class GameMaster;

class Environment {
private:
	Map* globalMap;
	Coordinates robotMapReferencePoint;

	bool appleHolder;
	bool bombHolder;
public:
	Environment();
	~Environment() = default;

	void setGlobalMap(Map* map) { this->globalMap = map; }
	Map* getGlobalMap() { return this->globalMap; }

	void setAppleHolder(bool status) { appleHolder = status; }
	bool getAppleHolder() { return appleHolder; }

	void setBombHolder(bool status) { bombHolder = status; }
	bool getBombHolder() { return bombHolder; }

	pair<Coordinates, CELL> scan(const Coordinates& coordinates);
	
	CELL getCell(const Coordinates& coordinates) const;
	CELL getCellInRobotWorld(const Coordinates& coordinates) const;

	bool cellIsEmpty(const Coordinates& coordinates) const;

	Coordinates getAbsoluteRobotCoodinates(const Coordinates& coordinates) const;
	Coordinates getRobotMapReferencePoint() const { return this->robotMapReferencePoint; };
};

#endif