#include "GameView.h"
#include "GameMaster.h"

/* char* ConsoleView::color(int color = 7, char* Message = "") {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    return Message;
} */

void GameView::drawMapForCertainRobot(Robot* robot, Map& M) {
    system("cls");

    if (robot->getCoordinates().x >= M.getVisibleGlobalMapX() - 1) {
        long long diff = robot->getCoordinates().x - M.getVisibleGlobalMapX() + 2;
        if (diff + robot->getCoordinates().x < M.getMapSizeX()) {
            this->expandFactorX = diff;
        }
    } else if(robot->getCoordinates().x < M.getVisibleGlobalMapX() - 1) {
        this->expandFactorX = 0;
    }

    if (robot->getCoordinates().y >= M.getVisibleGlobalMapY() - 1) {
        long long diff = robot->getCoordinates().y - M.getVisibleGlobalMapY() + 2;
        if (diff + robot->getCoordinates().y < M.getMapSizeY()) {
            this->expandFactorY = diff;
        }
    } else if (robot->getCoordinates().x < M.getVisibleGlobalMapX()) {
        this->expandFactorX = 0;
    }

    for (int i = this->expandFactorY; i < M.getVisibleGlobalMapX() + this->expandFactorY; ++i) {
        for (int j = this->expandFactorX; j < M.getVisibleGlobalMapY() + this->expandFactorY; ++j) {
            if (M.getMap()[i][j] == static_cast<int>(CELL::Cell_Empty)) {
                cout << " .";
            }
            else if (M.getMap()[i][j] == static_cast<int>(CELL::Cell_Rock)) {
                cout << " #";
            }
            else if (M.getMap()[i][j] == static_cast<int>(CELL::Cell_Apple)) {
                cout << " A";
            }
            else if (M.getMap()[i][j] == static_cast<int>(CELL::Cell_Bomb)) {
                cout << " !";
            }
            else if (M.getMap()[i][j] == static_cast<int>(CELL::Cell_Explorer)) {
                cout << " E";
            }
            else if (M.getMap()[i][j] == static_cast<int>(CELL::Cell_Sapper)) {
                cout << " S";
            }
            else if (M.getMap()[i][j] == static_cast<int>(CELL::Cell_Unknown)) {
                cout << " ?";
            }
        }
        cout << std::endl;
    }
}

