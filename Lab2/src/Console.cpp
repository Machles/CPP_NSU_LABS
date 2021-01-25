#include "Console.h"
#include <iostream>

COMMAND Console::getCommand(const string &command) {
    if(command == "reset"){ return COMMAND::RESET; }
    else if(command == "back"){ return COMMAND::BACK; }
    else if(command.substr(0, 5) == "step "){
        string box = command.substr(5);
        if(!isItValidNumber(box)){
            throw invalid_argument("Invalid number of steps.");
        }
        return COMMAND::STEP;
    }
    else if(command.substr(0, 4) == "set "){
        string box = command.substr(4);
        if(!isItCorrectCellCoordinates(box)){
            throw invalid_argument("Coordinates aren't correct.");
        }
        return COMMAND::SET;
    }
    else if(command.substr(0, 6) == "clear "){
        string box = command.substr(6);
        if(!isItCorrectCellCoordinates(box)){
            throw invalid_argument("Coordinates aren't correct.");
        }
        return COMMAND::CLEAR;
    }
    else if(command.substr(0, 5) == "load "){
        string box = command.substr(5);
        if(!isItCorrectFilename(box)){
            throw invalid_argument("Invalid filename.");
        }
        return COMMAND::LOAD;
    } else if(command.substr(0, 5) == "save "){
        string box = command.substr(5);
        if(!isItCorrectFilename(box)) {
            throw invalid_argument("Invalid filename.");
        }
        return COMMAND::SAVE;
    } else {
        throw invalid_argument("Invalid command");
    }
}

bool Console::isItValidNumber(const string &numberString) {
    // Only 0...9
    for(size_t i = 0; i < numberString.length(); i++){
        if( !(numberString[i] > 47 &&  numberString[i] < 58) ){
            return false;
        }
    }
    return true;
}

bool Console::isItCorrectCellCoordinates(const string & coordinates) {
    if(coordinates.length() != 2){
        return false;
    } else {
        if( !( (coordinates[1] > 47) && (coordinates[1] < 58) && (coordinates[0] > 64) && (coordinates[0] < 75) ) ) {
            return false;
        }
    }
    return true;
}

bool Console::isItCorrectFilename(const string &filename) {
    if(filename.substr(filename.length() - 4, filename.length() - 1) == ".txt"){
        return true;
    } else {
        return false;
    }
}

size_t Console::getNumberFromString(const string &numberString) {
    size_t number = 0;
    for(size_t i = 0; i < numberString.length(); i++){
        number = number * 10 + (numberString[i] - '0');
    }
    return number;
}