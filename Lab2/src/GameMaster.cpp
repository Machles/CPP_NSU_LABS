#include "GameMaster.h"
#include "Parser.h"

GameMaster::GameMaster() {
    this->stepCounter = 0;

    currentField = allocateMemoryForField(AXIS_SIZE);
    lastField = allocateMemoryForField(AXIS_SIZE);

    for(int i = 0; i < AXIS_SIZE; i++){
        for(int j = 0; j < AXIS_SIZE; j++) {
            currentField[i][j] = 0;
            lastField[i][j] = 0;
        }
    }
}

GameMaster::~GameMaster() {
    for(int i = 0; i < AXIS_SIZE; i++){
        delete[] currentField[i];
        delete[] lastField[i];
    }
}

void GameMaster::playGame(std::istream & stream){
    string currentCommand;
    Console console;
    Parser parser;
    while( !isGameOver ){
        getline(stream, currentCommand);
        try {
            switch (console.getCommand(currentCommand)) {
                case RESET: {
                    this->resetGame();
                    break;
                }
                case STEP: {
                    string box = currentCommand.substr(5);
                    size_t stepsAmount = console.getNumberFromString(box);
                    step(stepsAmount);
                    this->changeStepBackFlagStatus(true);
                    break;
                }
                case SET: {
                    string box = currentCommand.substr(4);
                    this->setCell(box[0] - 'A', box[1] - '0');
                    this->changeStepBackFlagStatus(true);
                    break;
                }
                case CLEAR: {
                    string box = currentCommand.substr(6);
                    this->clearCell(box[0] - 'A', box[1] - '0');
                    this->changeStepBackFlagStatus(true);
                    break;
                }
                case BACK: {
                    this->back();
                    break;
                }
                case LOAD: {
                    resetGame();
                    string box = currentCommand.substr(5);
                    parser.loadGameFromFile(this, box);
                    this->changeStepBackFlagStatus(false);
                    break;
                }
                case SAVE: {
                    string box = currentCommand.substr(5);
                    parser.saveGameIntoFile(getCurrentField(), box);
                    break;
                }
            }
        } catch (exception & ex) {
            cout << ex.what() << endl;
        }
        cout<<*this;
    }
}

bool ** GameMaster::allocateMemoryForField(const size_t & size) {
    bool ** field = new bool * [AXIS_SIZE];
    for(int i = 0; i < AXIS_SIZE; i++){
        field[i] = new bool [AXIS_SIZE];
    }
    return field;
}

size_t GameMaster::numCycle(const int & coordinate) const{
    if(coordinate == -1){ return AXIS_SIZE-1; }
    if(coordinate == AXIS_SIZE){ return 0; }
    return coordinate;
}

size_t GameMaster::countNeighbours(const size_t &x, const size_t &y, const bool & fieldType) const {
    size_t neighbourAmount = 0;

    if(fieldType == 0) {
        for(int i = -1; i < 2; i++){
            if( this->lastField[this->numCycle(x+i)][this->numCycle(y+1)] == 1) { neighbourAmount++; }
            if( this->lastField[this->numCycle(x+i)][this->numCycle(y)] == 1 && (i != 0)) { neighbourAmount++; }
            if( this->lastField[this->numCycle(x+i)][this->numCycle(y-1)] == 1) { neighbourAmount++; }
        }
    } else if(fieldType == 1){
        for(int i = -1; i < 2; i++){
            if( this->currentField[this->numCycle(9-y+i)][this->numCycle(x+1)] == 1) { neighbourAmount++; }
            if( this->currentField[this->numCycle(9-y+i)][this->numCycle(x)] == 1 && (i != 0)) { neighbourAmount++; }
            if( this->currentField[this->numCycle(9-y+i)][this->numCycle(x-1)] == 1) { neighbourAmount++; }
        }
    }


    return neighbourAmount;
}

void GameMaster::setCell(const size_t &x, const size_t &y) {
    for(int m = 0; m < AXIS_SIZE; m++){
        memcpy(lastField[m], currentField[m], sizeof(bool)*AXIS_SIZE  );
    }
    (this->currentField)[9-y][x] = 1;
    aliveCells++;
}

void GameMaster::clearCell(const size_t &x, const size_t &y) {
    for(int m = 0; m < AXIS_SIZE; m++){
        memcpy(lastField[m], currentField[m], sizeof(bool)*AXIS_SIZE  );
    }
    (this->currentField)[9-y][x] = 0;
    aliveCells--;
}

void GameMaster::step(const size_t & n){ // TODO - Without memcpy
    aliveCells = 0;
    size_t difAmount = 0;

    swap(currentField, lastField);

    for(size_t i = 0; i < n; i++){
        stepCounter++;
        for(size_t j = 0; j < AXIS_SIZE; j++){
            for(size_t l = 0; l < AXIS_SIZE; l++){
                currentField[j][l] = false;
                size_t neighboursAmount = countNeighbours(j, l, 0); // on lastField
                if(neighboursAmount == 2 && lastField[j][l] == 1 || neighboursAmount == 3 && lastField[j][l] == 1){
                    currentField[j][l] = true;
                } else if(lastField[j][l] == 0 && neighboursAmount == 3){
                    currentField[j][l] = true;
                    aliveCells++;
                } else if( (neighboursAmount < 2 || neighboursAmount > 3) && lastField[j][l] == 1) {
                    currentField[j][l] = false;
                    aliveCells--;
                }
                if(currentField[j][l] == 1){ aliveCells++; }
                if(currentField[j][l] != lastField[j][l] ){ difAmount++; }
            }
        }
        if(aliveCells == 0 || difAmount == 0){
            isGameOver = true;
        }
        if(i < n-1){ // На последнем степе, не будет копирования
            swap(currentField, lastField);
        }
    }

}

void GameMaster::resetGame(){
    this->stepCounter = 0;

    for(int i = 0; i < AXIS_SIZE; i++){
        for(int j = 0; j < AXIS_SIZE; j++) {
            currentField[i][j] = 0;
            lastField[i][j] = 0;
        }
    }
}

ostream & operator<< (std::ostream & out,  GameMaster & GM) {
    out << " ";
    for (int k = 0; k < AXIS_SIZE; k++) {
        out << " " << (char) (65 + k);
    }
    out << endl;
    for (int i = 0; i < AXIS_SIZE; i++) {
        out << 9 - i;
        for (int j = 0; j < AXIS_SIZE; j++) {
            if (GM.getCurrentField()[i][j] == 0) {
                out << " .";
            } else if (GM.getCurrentField()[i][j] == 1) {
                out << " *";
            }
        }
        out << endl;
    }
}

void GameMaster::back(){
    if(stepBackFlag){
        swap(currentField, lastField);
        stepBackFlag = false;
        if(stepCounter > 0){
            stepCounter--;
        }
    } else {
        cout << "You can't step back!" << endl;
    }
}
