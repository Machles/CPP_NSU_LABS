#include <iostream>
#include <sstream>
#include "GameMaster.h"
#include "Parser.h"

int main() {
    GameMaster * GM = new GameMaster();

    cout << *GM;

    GM->playGame(cin);

    return 0;
}
