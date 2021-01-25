#include "Receiver.h"
#include "Robot.h"

bool Receiver::isCellAvaliable(const Coordinates& coordinates) const{
	for (auto i : robotsCoordinates) {
		if (i.first == coordinates) {
			return false;
		}
	}
	return true;
}

void Receiver::notifyRobotCreated(Robot* robot, const Coordinates& coordinates) {
	robotsCoordinates.push_back(make_pair(coordinates, robot));
	vector<pair<Coordinates, CELL>> box;
	//robotsChanges.insert(std::make_pair(robot, box));
	robotsChanges.emplace(robot, box);
}

void Receiver::notifyRobotDeleted(Robot* robot, const Coordinates& coordinates) {
	for (size_t i = 0; i < robotsCoordinates.size(); ++i) {
		if (robotsCoordinates.at(i).first == coordinates) {
			robotsCoordinates.erase(robotsCoordinates.begin() + i);
		}
	}
	robotsChanges.erase(robot);
	actualMap->setCell(coordinates, CELL::Cell_Empty);
}

void Receiver::notifyCellScanned(Robot* robot, const pair<Coordinates, CELL> scannedCell) {
	robotsChanges.find(robot)->second.push_back(scannedCell);
	if (scannedCell.second == CELL::Cell_Apple)
		applesAvaliable.push_back(scannedCell.first);
	if (scannedCell.second == CELL::Cell_Bomb)
		bombsAvaliable.push_back(scannedCell.first);
}

void Receiver::notifyAppleCollected(Robot* robot, const Coordinates& coordinates) {
	// Response by key Robot* robot to map and hold changes
	robotsChanges.find(robot)->second.push_back(make_pair(coordinates, CELL::Cell_Empty));
	for (size_t i = 0; i < applesAvaliable.size(); ++i) {
		if (applesAvaliable.at(i) == coordinates) {
			applesAvaliable.erase(applesAvaliable.begin() + i);
			this->collectedApplesAmount++;
		}
	}
}

void Receiver::notifyBombDefused(Robot* robot, const Coordinates& coordinates) {
	// Response by key Robot* robot to map and hold changes
	robotsChanges.find(robot)->second.push_back(make_pair(coordinates, CELL::Cell_Empty));
	for (size_t i = 0; i < bombsAvaliable.size(); ++i) {
		if (bombsAvaliable.at(i) == coordinates) {
			bombsAvaliable.erase(bombsAvaliable.begin() + i);
		}
	}
}

void Receiver::notifyRobotMoved(const Robot* robot, const Coordinates& prevCoordinates, const Coordinates& newCoordinates) {
	for (size_t i = 0; i < robotsCoordinates.size(); ++i) {
		if (robotsCoordinates.at(i).first == prevCoordinates) {
			robotsCoordinates.at(i).first = newCoordinates;
		}
	}
}

void Receiver::applyOthersRobotsChanges() {
	for (auto i : robotsCoordinates) {
		Map* internalMap = i.second->getInternalMap();
		for (auto j : robotsChanges) {
			if (i.second != j.first) { // Do nothing for one robot
				for (auto k : j.second) {
					if (k.second == CELL::Cell_Empty && internalMap->getCell(k.first) != CELL::Cell_Apple) {
						internalMap->setCell(k.first, CELL::Cell_Empty);
					} if (k.second == CELL::Cell_Empty && internalMap->getCell(k.first) != CELL::Cell_Bomb) {
						internalMap->setCell(k.first, CELL::Cell_Empty);
					}else {
						internalMap->setCell(k.first, k.second);
					}
				}
			}
		}
		
		for (auto t : robotsCoordinates) {
			if (i.second != t.second) {
				if (dynamic_cast<Explorer*>(t.second) && internalMap->getCell(t.first) != CELL::Cell_Explorer) {
					internalMap->setCell(t.first, CELL::Cell_Explorer);
				} if (dynamic_cast<Sapper*>(t.second) && internalMap->getCell(t.first) != CELL::Cell_Sapper) {
					internalMap->setCell(t.first, CELL::Cell_Sapper);
				}
			}
		}

	}

	for (auto i = robotsChanges.begin(); i != robotsChanges.end(); ++i) {
		i->second.clear();
	}

	setActualMap(robotsCoordinates.at(0).second->getInternalMap());
}