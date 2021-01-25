#include "Parser.h"

void Parser::loadGameFromFile(GameMaster * GM, const string & filename) {
    Console console;
    ifstream readFile(filename);
    if(readFile.is_open()){
        string line;
        while(getline(readFile, line)){
            COMMAND getSETCommand = console.getCommand(line); // Здеся ловится
            string box = line.substr(4);
            if(getSETCommand != COMMAND::SET || !console.isItCorrectCellCoordinates(box)) {
                throw invalid_argument("Invalid format of data.");
            } else {
                GM->setCell(box[0] - 'A', box[1] - '0');
            }
        }
    }
    readFile.close();
}

void Parser::saveGameIntoFile(bool ** field, const string &filename) {
    ofstream saveFile(filename);
    if(saveFile.is_open()){
        for(int i = 0; i < AXIS_SIZE; i++){
            for(int j = 0; j < AXIS_SIZE; j++){
                if(field[i][j] == 1){
                    saveFile << "set " << (char) (j + 'A') << 9-i << '\n';
                }
            }
        }
    }
    saveFile.close();
}