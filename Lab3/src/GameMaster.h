#ifndef LAB3_GAMEMASTER_H
#define LAB3_GAMEMASTER_H

#include "Map.h"
#include "Mode.h"
#include "Robot.h"
#include "Receiver.h"
#include "Command.h"
#include "Parser.h"
#include "GameView.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <istream>
#include <exception>

using namespace std;

class GameMaster {
private:
    Map globalMap;
    Map * actualMap;

    unordered_map<string, Mode*> modes;
    vector<pair<Mode*, Robot*>> bundles;

    Command* currentCommand;
    Receiver* receiver;
    Parser* parser;
    GameView gameView;

public:
    GameMaster(int argc, char* argv[]);
    ~GameMaster();

    bool doGameStep();
    void play();

    auto getModes() { return &modes; }
    auto getBundles(char) { return &bundles; }
    auto getGlobalMap() { return &globalMap; }
    auto getParser() { return parser; }
    auto getReceiver() { return receiver; }

};


#endif //LAB3_GAMEMASTER_H