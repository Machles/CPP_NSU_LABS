#ifndef LAB3_MODE_H
#define LAB3_MODE_H

class ManualModeComManager;
class Command;
class Robot;
class GameMaster;

#include "Algorithms.h"

#include <vector>
#include <exception>

class Mode {
private:
	Command* command;
public:
	Mode() = default;
	~Mode() = default;
	virtual void setCommand(Command*& command);
	virtual bool execute(Robot * robot);
};

class ManualMode : public Mode {
private:
	Command* command;
public:
	ManualMode() = default;
	~ManualMode() = default;
	bool executeMMCommand(Robot* robot);
	void setCommand(Command*& command) override;
};

class IdleMode : public Mode {
public:
	IdleMode() = default;
	~IdleMode() = default;
	bool execute(Robot* robot) override;
};

class ScanMode : public Mode {
private:
	size_t steps;
	std::vector<Coordinates> scannerBox;
	size_t scannedCells;
public:
	ScanMode() = default;
	~ScanMode() = default;
	void setSteps(size_t steps) { this->steps = steps; }
	bool getPathToNearestUnknownCell(std::vector<Coordinates>& path, Robot* robot);
	void moveAlongPath(std::vector<Coordinates>& path, Robot * robot);
	bool execute(Robot* robot) override;
};

class AutoMode : public Mode {
public:
	AutoMode() = default;
	~AutoMode() = default;

	void getPathToObject(vector<Coordinates>& path, Robot* robot, const Coordinates& coord);
	void moveAlongPath(vector<Coordinates>& path, Robot* robot);
	bool execute(Robot* robot) override;
};

#endif //LAB3_MODE_H