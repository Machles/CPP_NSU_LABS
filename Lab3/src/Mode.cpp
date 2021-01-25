#include "Mode.h"
#include "Command.h"
#include "Robot.h"
#include "Parser.h"
#include "GameMaster.h"

void Mode::setCommand(Command*& command) {
	this->command = command;
}

bool Mode::execute(Robot* robot) {
	return true;
}

bool IdleMode::execute(Robot* robot){
	cout << "IDLE_MODE";
	return true;
}

bool ScanMode::execute(Robot* robot) {
	vector<Coordinates> path;
	dynamic_cast<Explorer*>(robot)->scanWorldAround();
	robot->updateMap();

	while (steps != 0) {
		if (getPathToNearestUnknownCell(path, robot)) {
			moveAlongPath(path, robot);
			robot->updateMap();
			steps--;
		}
		else {
			break;
		}
	}

	return true;
}

bool ScanMode::getPathToNearestUnknownCell(vector<Coordinates> &path, Robot* robot) {
	Coordinates startPoint = robot->getCoordinates();
	Coordinates endPoint = {-1, -1};

	std::unordered_map<Coordinates, Coordinates> came_from;
	std::unordered_map<Coordinates, long> cost_so_far;

	Algorithms algo;

	algo.dijikstra_algorithm(robot, startPoint, endPoint, CELL::Cell_Unknown, came_from, cost_so_far);
	if (endPoint.x == -1 && endPoint.y == -1)
		return false;
	path = algo.getPath(startPoint, endPoint, came_from, CELL::Cell_Unknown);
	return true;
}

void ScanMode::moveAlongPath(vector<Coordinates>& path, Robot* robot) {
	Coordinates diff = { 0, 0 };
	for (size_t i = 0; i < path.size() - 1; ++i) {
		diff = path.at(i + 1) - path.at(i);
		robot->move(robot->getDirection(diff));
		dynamic_cast<Explorer*>(robot)->scanWorldAround();
	}
}

void AutoMode::getPathToObject(vector<Coordinates>& path, Robot* robot, const Coordinates & coord) {
	Coordinates startPoint = robot->getCoordinates();
	Coordinates endPoint = coord;

	std::unordered_map<Coordinates, Coordinates> came_from;
	std::unordered_map<Coordinates, long> cost_so_far;

	Algorithms algo;

	algo.astar_algorithm(robot, startPoint, endPoint, came_from, cost_so_far);
	path = algo.getPath(startPoint, endPoint, came_from, robot->getInternalMap()->getCell(coord) );
}

void AutoMode::moveAlongPath(vector<Coordinates>& path, Robot* robot) {
	Coordinates diff = { 0, 0 };
	for (size_t i = 0; i < path.size() - 1; ++i) {
		diff = path.at(i + 1) - path.at(i);
		robot->move(robot->getDirection(diff));
	}
}

bool AutoMode::execute(Robot* robot) {
	vector<Coordinates> path;

	if (dynamic_cast<Explorer*>(robot)) {
		while (robot->getReceiver()->getApplesAvaliable()->size() != 0) {
			getPathToObject(path, robot, robot->getReceiver()->getApplesAvaliable()->at(0));
			moveAlongPath(path, robot);
			dynamic_cast<Explorer*>(robot)->grabApple();
			robot->updateMap();
		}
	}
	else if (dynamic_cast<Sapper*>(robot)) {
		while (robot->getReceiver()->getBombsAvaliable()->size() != 0) {
			getPathToObject(path, robot, robot->getReceiver()->getBombsAvaliable()->at(0));
			moveAlongPath(path, robot);
			dynamic_cast<Sapper*>(robot)->defuseBomb();
			robot->updateMap();
		}
	}

	return true;
}

void ManualMode::setCommand(Command*& command) {
	if (dynamic_cast<MMGrabCommand*>(command)) {
		this->command = dynamic_cast<MMGrabCommand*>(command);
	} if (dynamic_cast<MMScanCommand*>(command)) {
		this->command = dynamic_cast<MMScanCommand*>(command);
	} if (dynamic_cast<MMMoveCommand*>(command)) {
		this->command = dynamic_cast<MMMoveCommand*>(command);
	}
}

bool ManualMode::executeMMCommand(Robot* robot) {
	bool result = true;

	try {
		result &= dynamic_cast<ManualModeCommandManager*>(command)->execute(robot);
	} catch (exception& ex) {
		throw;
	}

	return result;
}