#ifndef LAB3_PARSER_H
#define LAB3_PARSER_H

#include "Command.h"
#include "Map.h"
#include "optionparser.h"
#include <cstdlib>
#include <string>
#include <sstream> 
#include <vector> 

class GameMaster;

class Parser {
private:
	std::string fileNameOfMap;
	int xSizeOfMap;
	int ySizeOfMap;
	size_t indexInParsedCom;
	std::vector<std::string> currentParsedCommand;
public:
	Parser();
	Command* parseCommand(GameMaster* gameMaster, bool isNeed);
	Command* getCurrentCommand(GameMaster* gameMaster, bool isNeed);

	std::string getNextWord();
	size_t getNextNumber();
	Direction decodeDirection(char dir);

	void clearCurrentParsedCommand();

	void setFileNameOfMap(int argc, char* argv[]);

	void split(const std::string& s, char delimiter);
	void getCommandLine(std::istream& cin);

	bool isValidMapSymbol(char& symbol);

	void getMapFromFile(Map& map);
};

#endif //LAB3_PARSER_H