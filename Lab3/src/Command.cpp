#include "Command.h"
#include "Mode.h"
#include "GameMaster.h"

ChangeModeCommand::ChangeModeCommand(std::string modeName, GameMaster* gameMaster, size_t steps) {
	this->modeName = modeName;
	this->gameMaster = gameMaster;
	this->steps = steps;
}

ChangeModeCommand::ChangeModeCommand(std::string mode, GameMaster* gameMaster) {
	this->modeName = mode;
	this->gameMaster = gameMaster;
}

bool ChangeModeCommand::execute() {
	auto modes = gameMaster->getModes();
	Mode* currentMode = nullptr;
	currentMode = (modes->find(this->modeName))->second;
	auto bundles = gameMaster->getBundles('!');
	for (size_t i = 0; i < bundles->size(); i++) {
		if (dynamic_cast<Explorer*>(bundles->at(i).second)) {
			bundles->at(i).first = currentMode;
			if (this->modeName == "SCAN") {
				dynamic_cast<ScanMode*>(currentMode)->setSteps(this->steps);
			}
		}
		else if (dynamic_cast<Sapper*>(bundles->at(i).second) && this->modeName == "AUTO") {
			bundles->at(i).first = currentMode;
		}
	}
	return true;
}

bool MMMoveCommand::execute(Robot* robot) {
	try {
		robot->move(this->dir);
	}
	catch (exception& ex) {
		throw;
	}
	return true;
}

bool MMScanCommand::execute(Robot* robot) {
	dynamic_cast<Explorer*>(robot)->scanWorldAround();
	return true;
}

bool MMGrabCommand::execute(Robot* robot) {
	dynamic_cast<Explorer*>(robot)->grabApple();
	return true;
}

bool Command::execute(){
	std::cout << "Execute";
	return true;
}

SapperManagerCommand::SapperManagerCommand(std::string state, GameMaster* gameMaster) {
	this->state = state;
	this->gameMaster = gameMaster;
}

bool SapperManagerCommand::execute() {
	auto bundles = gameMaster->getBundles('!');
	if (state == "ON") {
		auto modes = gameMaster->getModes();
		Sapper * sp = new Sapper(gameMaster->getGlobalMap(), gameMaster->getReceiver());
		bundles->push_back(std::make_pair((modes->find("IDLE"))->second, sp));
	} else if(state == "OFF") {
		for (size_t i = 0; i < bundles->size(); i++) {
			if (dynamic_cast<Sapper*>(bundles->at(i).second)) {
				Robot* ref = bundles->at(i).second;
				bundles->erase(bundles->begin() + i);
				delete dynamic_cast<Sapper*>(ref);
			}
		}
	}
	return true;
}

QuitCommand::QuitCommand(GameMaster* gameMaster) {
	this->gameMaster = gameMaster;
}

bool QuitCommand::execute() {
	return true;
}