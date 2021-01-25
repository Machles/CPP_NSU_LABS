#ifndef LAB3_COMMAND_H
#define LAB3_COMMAND_H

#include "Robot.h"
#include "Direction.h"
#include <string>
#include <exception>

class GameMaster;
class Mode;

class Command {
public:
	virtual bool execute();
};

class ChangeModeCommand : public Command {
private:
	std::string modeName;
	GameMaster* gameMaster;
	size_t steps = 0;
public:
	virtual ~ChangeModeCommand() = default;
	ChangeModeCommand(std::string modeName, GameMaster* gameMaster, size_t steps);
	ChangeModeCommand(std::string mode, GameMaster* gameMaster);
	bool execute() override;
};

class ManualModeCommandManager : public Command {
public:
	virtual bool execute(Robot* robot) = 0; // Абстрактная функция
};

class MMMoveCommand : public ManualModeCommandManager {
private:
	Direction dir;
public:
	MMMoveCommand(Direction direction) { dir = direction; };
	bool execute(Robot* robot) override;
};

class MMGrabCommand : public ManualModeCommandManager {
public:
	MMGrabCommand() = default;
	bool execute(Robot* robot) override;
};

class MMScanCommand : public ManualModeCommandManager {
public:
	MMScanCommand() = default;
	bool execute(Robot* robot) override;
};

class SapperManagerCommand : public Command{
private:
	std::string state;
	GameMaster* gameMaster;
public:
	SapperManagerCommand(std::string state, GameMaster* gameMaster);
	virtual bool execute() override;
};

class QuitCommand : public Command {
private:
	GameMaster* gameMaster;
public:
	QuitCommand(GameMaster* gameMaster);
	virtual bool execute() override;
};

#endif //LAB3_COMMAND_H