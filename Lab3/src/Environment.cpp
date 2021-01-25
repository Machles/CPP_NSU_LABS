#include "GameMaster.h"
#include "Environment.h"

Environment::Environment() {
	this->appleHolder = 0;
	this->bombHolder = 0;
	this->robotMapReferencePoint = {0, 0};
	this->globalMap = nullptr;
	this->appleHolder = 0;
}

Coordinates Environment::getAbsoluteRobotCoodinates(const Coordinates& coordinates) const {
	return { robotMapReferencePoint.x + coordinates.x, robotMapReferencePoint.y + coordinates.y };
}

CELL Environment::getCell(const Coordinates& coordinates) const {
	return globalMap->getCell(coordinates);
} // Uses Global SK

CELL Environment::getCellInRobotWorld(const Coordinates& coordinates) const {
	return globalMap->getCell(getAbsoluteRobotCoodinates(coordinates));
} // Uses Robot's Internal Map SK

bool Environment::cellIsEmpty(const Coordinates& coordinates) const{
	return (getCell(coordinates) == CELL::Cell_Empty) ? 1 : 0;
}