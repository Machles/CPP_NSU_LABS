#include "Algorithms.h"

bool Algorithms::dijikstra_algorithm(Robot* robot, const Coordinates& startPoint, 
	Coordinates& endPoint, CELL goalCell, std::unordered_map<Coordinates,
	Coordinates>& came_from, std::unordered_map<Coordinates, long>& cost_so_far) {
	
	PriorityQueue<Coordinates, long> box;
	box.put(startPoint, 0);
	came_from[startPoint] = startPoint;
	cost_so_far[startPoint] = 0;
	vector <Coordinates> cellNeighbours;

	while (!box.empty()) {
		Coordinates curr = box.get();
		cellNeighbours = getCellNeighbours(robot->getInternalMap(), curr);

		if (robot->getInternalMap()->getCell(curr) == goalCell) {
			endPoint = curr;
			break;
		}

		for (Coordinates next : cellNeighbours) {

			if (robot->getEnvironment()->getCellInRobotWorld(next) == CELL::Cell_Empty ||
				robot->getEnvironment()->getCellInRobotWorld(next) == CELL::Cell_Apple) {

				long newCost = cost_so_far[curr] + getCostForCell(robot->getInternalMap(), next, robot);
				if ((cost_so_far.find(next) == cost_so_far.end() || newCost < cost_so_far[next])) {
					cost_so_far[next] = newCost;
					came_from[next] = curr;
					box.put(next, newCost);
				}

			}
			
		}

	}

	return 0;
}

bool Algorithms::astar_algorithm(Robot* robot, const Coordinates& startPoint,
	const Coordinates& endPoint, std::unordered_map<Coordinates,
	Coordinates>& came_from, std::unordered_map<Coordinates, long>& cost_so_far) {

	PriorityQueue<Coordinates, long> box;
	box.put(startPoint, 0);
	came_from[startPoint] = startPoint;
	cost_so_far[startPoint] = 0;
	vector <Coordinates> cellNeighbours;

	while (!box.empty()) {
		Coordinates curr = box.get();
		cellNeighbours = getCellNeighbours(robot->getInternalMap(), curr);

		if (curr == endPoint) {
			break;
		}

		for (Coordinates next : cellNeighbours) {

			if (dynamic_cast<Explorer*>(robot) &&
				(robot->getEnvironment()->getCellInRobotWorld(next) == CELL::Cell_Empty ||
				robot->getEnvironment()->getCellInRobotWorld(next) == CELL::Cell_Apple) ) {

				long newCost = cost_so_far[curr] + getCostForCell(robot->getInternalMap(), next, robot);
				if ((cost_so_far.find(next) == cost_so_far.end() || newCost < cost_so_far[next])) {
					cost_so_far[next] = newCost;
					long priority = newCost + heuristic(next, endPoint);
					came_from[next] = curr;
					box.put(next, priority);
				}
			
			}

			if (dynamic_cast<Sapper*>(robot) &&
				(robot->getEnvironment()->getCellInRobotWorld(next) == CELL::Cell_Empty ||
				robot->getEnvironment()->getCellInRobotWorld(next) == CELL::Cell_Apple ||
				robot->getEnvironment()->getCellInRobotWorld(next) == CELL::Cell_Bomb) ) {

				long newCost = cost_so_far[curr] + getCostForCell(robot->getInternalMap(), next, robot);
				if ((cost_so_far.find(next) == cost_so_far.end() || newCost < cost_so_far[next])) {
					cost_so_far[next] = newCost;
					long priority = newCost + heuristic(next, endPoint);
					came_from[next] = curr;
					box.put(next, priority);
				}

			}
		}

	}

	return 0;
}

long Algorithms::heuristic(const Coordinates& a, const Coordinates& b) {
	return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

std::vector<Coordinates> Algorithms::getPath(const Coordinates& startPoint, const Coordinates& endPoint, std::unordered_map<Coordinates, Coordinates> came_from, CELL type) {
	std::vector<Coordinates> path;
	Coordinates curr = endPoint;

	if (type != CELL::Cell_Unknown) {
		path.push_back(curr);
	}
	curr = came_from[curr];

	while (curr != startPoint) {
		path.push_back(curr);
		curr = came_from[curr];
	}
	
	path.push_back(startPoint);
	std::reverse(path.begin(), path.end());
	return path;
}


int Algorithms::getCostForCell(Map* robotInternalMap, const Coordinates& cellCoordinates, Robot * robot) {
	
	if (dynamic_cast<Sapper*>(robot)) {
		switch (robotInternalMap->getCell(cellCoordinates)) {
		case CELL::Cell_Empty: return 1;
		case CELL::Cell_Apple: return 1;
		case CELL::Cell_Rock: return INT_MAX;
		case CELL::Cell_Bomb: return 1;
		case CELL::Cell_Explorer: return INT_MAX;
		case CELL::Cell_Sapper: return INT_MAX;
		}
	}
	else if (dynamic_cast<Explorer*>(robot)) {
		switch (robotInternalMap->getCell(cellCoordinates)) {
		case CELL::Cell_Empty: return 1;
		case CELL::Cell_Apple: return 1;
		case CELL::Cell_Rock: return INT_MAX;
		case CELL::Cell_Bomb: return INT_MAX;
		case CELL::Cell_Explorer: return INT_MAX;
		case CELL::Cell_Sapper: return INT_MAX;
		}
	}
	
}

vector<Coordinates> Algorithms::getCellNeighbours(Map* robotInternalMap, const Coordinates& cellCoordinates) {
	vector <Coordinates> cellNeighbours;
	Coordinates box;

	box = { cellCoordinates.x + 1, cellCoordinates.y };
	if (!robotInternalMap->isCoordinatesOutOfMap(box)) {
		cellNeighbours.push_back(box);
	}

	box = { cellCoordinates.x, cellCoordinates.y + 1 };
	if (!robotInternalMap->isCoordinatesOutOfMap(box)) {
		cellNeighbours.push_back(box);
	}

	box = { cellCoordinates.x - 1, cellCoordinates.y };
	if (!robotInternalMap->isCoordinatesOutOfMap(box)) {
		cellNeighbours.push_back(box);
	}

	box = { cellCoordinates.x, cellCoordinates.y - 1 };
	if (!robotInternalMap->isCoordinatesOutOfMap(box)) {
		cellNeighbours.push_back(box);
	}


	return cellNeighbours;
}
