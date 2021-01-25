#include <iostream>
#include "src/GameView.h"
#include "src/GameMaster.h"
#include "src/Map.h"
#include "src/optionparser.h"
#include "src/Parser.h"

int main(int argc, char* argv[]) {
    GameMaster* gm = new GameMaster(argc, argv);
    gm->play();

    gm->getParser()->setFileNameOfMap(argc, argv);
    cout << gm->getParser()->getFileNameOfMap();

    return 0;
}