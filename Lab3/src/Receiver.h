#ifndef LAB3_RECEIVER_H
#define LAB3_RECEIVER_H

#include <vector>
#include <map>
#include "Coordinates.h"
#include "Map.h"

using namespace std;

class Robot;

class Receiver {
private:
	Map* actualMap;
	vector<pair<Coordinates, Robot*>> robotsCoordinates;

	size_t collectedApplesAmount;

	vector<Coordinates> applesAvaliable;
	vector<Coordinates> bombsAvaliable;

	map<Robot*, vector<pair<Coordinates, CELL>>> robotsChanges;
public:
	Receiver(Map* actualMap) : actualMap(actualMap), collectedApplesAmount(0) {};
	Receiver() : actualMap(nullptr), collectedApplesAmount(0) {}
	~Receiver() = default;

	bool isCellAvaliable(const Coordinates& coordinates) const;

	size_t getCollectedApplesAmount() { return collectedApplesAmount; };

	auto getApplesAvaliable() { return &applesAvaliable; };
	auto getBombsAvaliable() { return &bombsAvaliable; };

	void setActualMap(Map* actualMap) { this->actualMap = actualMap; }
	Map* getActualMap() { return this->actualMap; }

	void notifyRobotCreated(Robot* robot, const Coordinates& coordinates);
	void notifyRobotDeleted(Robot* robot, const Coordinates& coordinates);

	void notifyAppleCollected(Robot* robot, const Coordinates& coordinates);
	void notifyBombDefused(Robot* robot, const Coordinates& coordinates);
	void notifyRobotMoved(const Robot* robot, const Coordinates& prevCoordinates, const Coordinates& newCoordinates);
	void notifyCellScanned(Robot* robot, const pair<Coordinates, CELL> scannedCell);

	void applyOthersRobotsChanges();

};

#endif