#include "Parser.h"
#include "GameMaster.h"

Parser::Parser() : indexInParsedCom(0), xSizeOfMap(15), ySizeOfMap(15) {}

Command* Parser::getCurrentCommand(GameMaster* gameMaster, bool isNeed) {
	getCommandLine(cin);
	Command* currentCommand = parseCommand(gameMaster, isNeed);
	return currentCommand;
}

Command* Parser::parseCommand(GameMaster* gameMaster, bool isNeed) {
	Command* currentCommand = nullptr;
	// Первый робот в режиме ManualMode
	int box = gameMaster->getBundles('!')->size();
	Robot* robot = gameMaster->getBundles('!')->at(0).second;
	Mode* currentMode = gameMaster->getBundles('!')->at(0).first;
	string word = "";
	size_t number = 0;
	
	if (isNeed) {
		word = getNextWord();
		if (word == "SET_MODE") {
			word = getNextWord();
			if (word == "MANNUAL" || word == "AUTO") {
				currentCommand = new ChangeModeCommand(word, gameMaster);
			}
			else if (word == "SCAN") {
				number = getNextNumber();
				currentCommand = new ChangeModeCommand(word, gameMaster, number);
			}
		} else if (word == "SAPPER") {
			word = getNextWord();
			if (word == "ON" || word == "OFF") {
				currentCommand = new SapperManagerCommand(word, gameMaster);
			}
		} else if (word == "QUIT") {
			currentCommand = new QuitCommand(gameMaster);
		} else if(dynamic_cast<ManualMode*>(currentMode)){
			if (word == "GRAB") {
				currentCommand = new MMGrabCommand();
			} else if (word == "MOVE") {
				word = getNextWord();
				if (word == "U" || word == "R" || word == "L" || word == "D") {
					Direction dir = decodeDirection(word[0]);
					currentCommand = new MMMoveCommand(dir);
				}
			} else if (word == "SCAN") {
				currentCommand = new MMScanCommand();
			} 
		}
	}

	clearCurrentParsedCommand();
	return currentCommand;
}

void Parser::clearCurrentParsedCommand() {
	indexInParsedCom = 0;
	currentParsedCommand.clear();
}

Direction Parser::decodeDirection(char dir) {
	switch (dir) {
		case 'U': return Direction::Direction_Up;
		case 'D': return Direction::Direction_Down;
		case 'L': return Direction::Direction_Left;
		case 'R': return Direction::Direction_Right;
	}
}

string Parser::getNextWord() {
	std::string word = currentParsedCommand.at(indexInParsedCom);
	indexInParsedCom++;
	return word;
}

size_t Parser::getNextNumber() {
	size_t number = 0;
	std::string word = currentParsedCommand.at(indexInParsedCom);
	for (size_t i = 0; i < word.length(); ++i) {
		number = number * 10 + (word[i] - '0');
	}
	return number;
}

void Parser::split(const std::string& s, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream (s);
	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}
	currentParsedCommand = tokens;
}

void Parser::getCommandLine(std::istream& cin) {
	string box = "";
	getline(cin, box);
	split(box, ' ');
}

void Parser::setFileNameOfMap(int argc, char* argv[]) {
	argc -= (argc > 0);
	argv += (argc > 0); // Skip filename
	const option::Descriptor usage[] = { {0, 0, nullptr, nullptr, nullptr, nullptr } };
	option::Stats stats(usage, argc, argv);

	std::vector<option::Option> options(stats.options_max);
	std::vector<option::Option> buffer(stats.buffer_max);
	option::Parser parse(usage, argc, argv, &options[0], &buffer[0]);

	if (parse.error()) {
		throw exception("Error of parsing arguments");
	} 

	if (parse.nonOptionsCount() != 1) {
		throw exception("Amount of arguments isn't correct");
	}

	this->fileNameOfMap = parse.nonOption(0);
}

bool Parser::isValidMapSymbol(char& symbol) {
	switch (symbol) {
		case 'R': return true;
		case 'B': return true;
		case 'E': return true;
		case 'A': return true;
	}
}

void Parser::getMapFromFile(Map& map) {

	ifstream file("D:\\NSU\\LABS_OOOP_CPP\\Lab3\\RobotGame\\maps\\" + this->fileNameOfMap); 
	string buffer;

	file >> xSizeOfMap;
	file >> ySizeOfMap;

	map.setMapSizeX(xSizeOfMap);
	map.setMapSizeY(ySizeOfMap);
	map.createEmptyMap();

	getline(file, buffer);

	for (int i = 0; i < ySizeOfMap; ++i) {
		getline(file, buffer);
		for (int j = 0; j < xSizeOfMap; ++j) {
			if (isValidMapSymbol(buffer[j])) {
				switch (buffer[j]) {
				case 'R': map.setCell({ j, i }, CELL::Cell_Rock); break;
				case 'B': map.setCell({ j, i }, CELL::Cell_Bomb); break;
				case 'E': map.setCell({ j, i }, CELL::Cell_Empty); break;
				case 'A': map.setCell({ j, i }, CELL::Cell_Apple); break;
				}
			}
			else {
				throw invalid_argument("Map encoded in invalid format!");
			}
		}
	}
}