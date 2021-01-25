#ifndef LAB3_GAMEVIEW_H
#define LAB3_GAMEVIEW_H

#include <iostream>
#include <cstdlib>
#include "Map.h"
#include "Robot.h"

class GameMaster;

class GameView {
	int expandFactorX;
	int expandFactorY;
public:
	GameView() : expandFactorX(0), expandFactorY(0) {}
	char* color(int color, char* Message);
	void drawMapForCertainRobot(Robot* robot, Map& M);
};

/*
1: Blue
2: Green
3: Cyan
4: Red
5: Purple
6: Yellow (Dark)
7: Default white
8: Gray/Grey
9: Bright blue
10: Brigth green
11: Bright cyan
12: Bright red
13: Pink/Magenta
14: Yellow
15: Bright white
*/

#endif //LAB3_GAMEVIEW_H