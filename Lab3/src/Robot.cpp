#include "Robot.h"

Robot::Robot() : environment(new Environment()), receiver(nullptr), position({ 0,0 }), intMap(nullptr) {}

Robot::~Robot() {
	//delete environment;
	delete intMap;
}

void Explorer::move(const Direction& direction) {
	Coordinates newCoord = createNewCoordinates(direction);
	if (newCoord.x < 0 || newCoord.y < 0) {
		throw invalid_argument("Invalid coordinates, because RPofIntMap {0, 0} is the same as RPofGlMap");
	}
	CELL obj = this->intMap->getCell(newCoord);
	if (isRobotAbleToStep(newCoord, this) && this->intMap->getCell(newCoord) != CELL::Cell_Unknown) {
		if (this->environment->getAppleHolder() == true) {
			this->intMap->setCell(this->position, CELL::Cell_Apple);
			this->environment->setAppleHolder(false);
		} else {
			this->intMap->setCell(this->position, CELL::Cell_Empty);
		}
		if (obj == CELL::Cell_Apple) {
			this->environment->setAppleHolder(true);
		}
		this->intMap->setCell(newCoord, CELL::Cell_Explorer);
		this->receiver->notifyRobotMoved(this, this->position, newCoord);
		this->position = newCoord;
		updateMap();
	}
}

void Sapper::move(const Direction& direction) {
	Coordinates newCoord = createNewCoordinates(direction);
	if (newCoord.x < 0 || newCoord.y < 0) {
		throw invalid_argument("Invalid coordinates, because RPofIntMap {0, 0} is the same as RPofGlMap");
	}
	CELL obj = this->intMap->getCell(newCoord);
	if (isRobotAbleToStep(newCoord, this) && this->intMap->getCell(newCoord) != CELL::Cell_Unknown) {
		if (this->environment->getAppleHolder() == true) {
			this->intMap->setCell(this->position, CELL::Cell_Apple);
			this->environment->setAppleHolder(false);
		} else if (this->environment->getBombHolder() == true) {
			this->intMap->setCell(this->position, CELL::Cell_Bomb);
			this->environment->setBombHolder(false);
		} else {
			this->intMap->setCell(this->position, CELL::Cell_Empty);
		}
		if (obj == CELL::Cell_Apple) {
			this->environment->setAppleHolder(true);
		} else if (obj == CELL::Cell_Bomb) {
			this->environment->setBombHolder(true);
		}
		this->intMap->setCell(newCoord, CELL::Cell_Sapper);
		this->receiver->notifyRobotMoved(this, this->position, newCoord);
		this->position = newCoord;
	}
	else {
		throw("Robot can't move in this direction!");
	}
}

Coordinates Robot::getValidCoordinatesForRobotSpawn() {
	Coordinates box = { 0, 0 };
	size_t cntrX = 0;
	size_t cntrY = 0;
	while(!isRobotAbleToStep(box, this)){
		++cntrX;
		box.setXY(cntrX, cntrY);
		if (cntrX == this->environment->getGlobalMap()->getMapSizeX()) {
			cntrX = 0;
			++cntrY;
		}
	}
	//if (this->environment->getCell(box) == CELL::Cell_Apple) {
		//this->environment->setAppleHolder(true);
		//this->receiver->notifyCellScanned(this, make_pair(box, CELL::Cell_Apple));
	//}
	/*if (this->environment->getCell(box) == CELL::Cell_Bomb) {
		this->environment->setBombHolder(true);
		this->receiver->notifyCellScanned(this, make_pair(box, CELL::Cell_Bomb));
	}*/

	return box;
}

Direction Robot::getDirection(const Coordinates& diff) {
	if(diff.x == 0){
		if (diff.y == 1)
			return Direction::Direction_Down;
		if (diff.y == -1)
			return Direction::Direction_Up;
	}

	if (diff.y == 0) {
		if (diff.x == 1)
			return Direction::Direction_Right;
		if (diff.x == -1)
			return Direction::Direction_Left;
	}
}

bool Robot::isRobotAbleToStep(const Coordinates& coordinates, Robot * robot) const {
	CELL cellType = environment->getCellInRobotWorld(coordinates);

	if ( (cellType == CELL::Cell_Empty || cellType == CELL::Cell_Apple || (dynamic_cast<Sapper*>(robot) && cellType == CELL::Cell_Bomb)) &&
		!intMap->isCoordinatesOutOfMap(coordinates)) {
		return true;
	}

	return false;
}

void Robot::updateMap() { // For 1 robot
	this->receiver->applyOthersRobotsChanges();
}

Coordinates Robot::createNewCoordinates(const Direction& direction) {
	Coordinates coord = this->position;

	switch (direction) {
	case Direction::Direction_Up: coord.y = (this->position.y - 1); break;
	case Direction::Direction_Down: coord.y = (this->position.y + 1); break;
	case Direction::Direction_Left: coord.x = (this->position.x - 1); break;
	case Direction::Direction_Right: coord.x = (this->position.x + 1); break;
	}

	return coord;
}

Explorer::Explorer(Map* globalMap, Receiver* receiver) {
	this->environment->setGlobalMap(globalMap);
	this->setReceiver(receiver);

	this->intMap = new Map(globalMap->getMapSizeX(), globalMap->getMapSizeY());

	this->position = getValidCoordinatesForRobotSpawn();
	this->intMap->setCell(this->position, CELL::Cell_Explorer);
	this->receiver->notifyRobotCreated( this , this->position);
}

void Explorer::grabApple() {
	if (this->environment->getAppleHolder() == 1) {
		intMap->setAppleAmount(intMap->getAppleAmount() + 1);
		this->environment->setAppleHolder(false);
		this->receiver->notifyAppleCollected(this, this->position);
	}
}

void Explorer::subScan(const int& xAdd, const int& yAdd) {
	Coordinates box = { this->position.x + xAdd, this->position.y + yAdd };
	if (!intMap->isCoordinatesOutOfMap(box) &&
		this->intMap->getCell(box) == CELL::Cell_Unknown) {
		CELL cellOnGlobalMap = this->environment->getCellInRobotWorld(box);
		this->intMap->setCell(box, cellOnGlobalMap);
		this->receiver->notifyCellScanned(this, std::make_pair(box, cellOnGlobalMap) );
	}
}

void Explorer::scanWorldAround() {
	subScan(1, 0);
	subScan(0, 1);
	subScan(0, -1);
	subScan(-1, 0);
	updateMap();
}

Explorer::~Explorer() {
	this->receiver->notifyRobotDeleted(this, this->position);
}

void Sapper::defuseBomb() {
	if (this->environment->getBombHolder() == 1) {
		intMap->setBombAmount(intMap->getBombAmount() + 1);
		this->environment->setBombHolder(false);
		this->receiver->notifyBombDefused(this, this->position);
	}
}

Sapper::Sapper(Map* globalMap, Receiver* receiver) {
	this->environment->setGlobalMap(globalMap);
	this->setReceiver(receiver);

	this->intMap = new Map(globalMap->getMapSizeX(), globalMap->getMapSizeY());

	this->position = getValidCoordinatesForRobotSpawn();
	this->intMap->setCell(this->position, CELL::Cell_Sapper);

	this->receiver->notifyRobotCreated(this, this->position);
	this->setInternalMap( this->receiver->getActualMap() ); // ТАк, тут разобраться

	updateMap();
}

Sapper::~Sapper() {
	this->receiver->notifyRobotDeleted(this, this->position);
}