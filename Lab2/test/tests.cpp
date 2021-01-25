#include "gtest/gtest.h"
#include "../src/Console.h"
#include "../src/Parser.h"
#include "../src/GameMaster.h"

class GameMasterFixture : public :: testing:: Test {

protected:
    virtual void TearDown(){
        delete GM;
    }

    virtual void SetUp(){
        GM = new GameMaster();
    }

    GameMaster * GM;
    Parser prsr;

};

class ConsoleFixture : public :: testing:: Test {
protected:
    Console cnsl;
};

class ParserFixture : public :: testing:: Test {
protected:
    Parser prsr;
};

TEST_F(ConsoleFixture, isGetCommandCorrectWorking_1 ){
    EXPECT_EQ(SET,cnsl.getCommand("set A5"));
    EXPECT_EQ(CLEAR,cnsl.getCommand("clear A5"));
    EXPECT_EQ(BACK,cnsl.getCommand("back"));
    EXPECT_EQ(RESET,cnsl.getCommand("reset"));
    EXPECT_EQ(LOAD,cnsl.getCommand("load filename.txt"));
    EXPECT_EQ(SAVE,cnsl.getCommand("save filename.txt"));
}

TEST_F(ConsoleFixture, isGetCommandCorrectWorking_2 ){
    EXPECT_THROW({ cnsl.getCommand("set 5A"); }, exception );
    EXPECT_THROW({ cnsl.getCommand("set a5"); }, exception );
    EXPECT_THROW({ cnsl.getCommand("set aa"); }, exception );
    EXPECT_THROW({ cnsl.getCommand("set 55"); }, exception );
    EXPECT_THROW({ cnsl.getCommand("set a"); }, exception );

    EXPECT_THROW({ cnsl.getCommand("clear 7B"); }, exception );
    EXPECT_THROW({ cnsl.getCommand("clear b7"); }, exception );
    EXPECT_THROW({ cnsl.getCommand("clear bb"); }, exception );
    EXPECT_THROW({ cnsl.getCommand("clear 77"); }, exception );
    EXPECT_THROW({ cnsl.getCommand("clear b"); }, exception );

    EXPECT_THROW({ cnsl.getCommand("load filename"); }, exception );
    EXPECT_THROW({ cnsl.getCommand("load filename.doc"); }, exception );
    EXPECT_THROW({ cnsl.getCommand("load filename."); }, exception );
    EXPECT_THROW({ cnsl.getCommand("load filename.tx"); }, exception );
    EXPECT_THROW({ cnsl.getCommand("load filename.txx"); }, exception );

    EXPECT_THROW({ cnsl.getCommand("save filename"); }, exception );
    EXPECT_THROW({ cnsl.getCommand("save filename.doc"); }, exception );
    EXPECT_THROW({ cnsl.getCommand("save filename.tx"); }, exception );
    EXPECT_THROW({ cnsl.getCommand("save filename.txx"); }, exception );

    EXPECT_THROW({ cnsl.getCommand("step five"); }, exception );
    EXPECT_THROW({ cnsl.getCommand("step 34s34"); }, exception );

    EXPECT_THROW({ cnsl.getCommand("bck"); }, exception );
    EXPECT_THROW({ cnsl.getCommand("rst"); }, exception );
}

TEST_F(GameMasterFixture, isSetCellWorkingCorrectly){
    for(int i = 0; i < AXIS_SIZE; i++){
        for(int j = 0; j < AXIS_SIZE; j++){
            EXPECT_EQ(false, GM->getCurrentField()[i][j]);
        }
    }

    GM->setCell(9, 0); // J0

    for(int i = 0; i < AXIS_SIZE; i++){
        for(int j = 0; j < AXIS_SIZE; j++){
            if(i != 9 && j != 9){
                EXPECT_EQ(false, GM->getCurrentField()[i][j]);
            } else if(i == 9 && j == 9) { // Coordinates of J0 in array
                EXPECT_EQ(true, GM->getCurrentField()[i][j]);
            }
        }
    }
}

TEST_F(GameMasterFixture, isClearCellWorkingCorrectly){

    for(int i = 0; i < AXIS_SIZE; i++){
        for(int j = 0; j < AXIS_SIZE; j++){
            EXPECT_EQ(false, GM->getCurrentField()[i][j]);
        }
    }

    GM->setCell(9, 0); // J0
    GM->clearCell(9, 0); // J0

    for(int i = 0; i < AXIS_SIZE; i++){
        for(int j = 0; j < AXIS_SIZE; j++){
            EXPECT_EQ(false, GM->getCurrentField()[i][j]);
        }
    }
}

TEST_F(GameMasterFixture, isCountNeighboursWorkingCorrectly){
    GM->setCell(9, 0); // J0

    GM->setCell(0, 9); // A9
    GM->setCell(0, 1); // A1
    GM->setCell(0, 0); // A0
    GM->setCell(9, 9); // J9
    GM->setCell(8, 9); // I9

    GM->setCell(9, 1); // J1
    GM->setCell(8, 1); // I1
    GM->setCell(8, 0); // I0

    EXPECT_EQ(8, GM->countNeighbours(9, 0, 1)); // Checked all extreme situations
}

TEST_F(GameMasterFixture, isResetGameWorkingCorrectly){
    ostringstream os1;
    ostringstream os2;
    os1 << *GM;

    for(int i = 0; i < AXIS_SIZE; i++){
        for(int j = 0; j < AXIS_SIZE; j++){
            GM->setCell(i, j);
        }
    }

    GM->resetGame();

    os2 << *GM;

    string out1 = os1.str();
    string out2 = os2.str();

    EXPECT_EQ(1, out1 == out2);
}

TEST_F(GameMasterFixture, isStepWorkingCorrectly){
    ostringstream osState1;
    ostringstream osState2;
    ostringstream osCurrentFieldState;

    // Mill
    GM->setCell(5, 6);  // *
    GM->setCell(6, 6);  // *
    GM->setCell(7, 6);  // *
    osState1 << *GM;
    string state1 = osState1.str();

    GM->resetGame();

    GM->setCell(6, 6); //
    GM->setCell(6, 5); // ***
    GM->setCell(6, 7); //
    osState2 << *GM;
    string state2 = osState2.str();

    GM->step(1);
    osCurrentFieldState << *GM;
    string step1 = osCurrentFieldState.str();
    osCurrentFieldState.str("");

    GM->step(1);
    osCurrentFieldState << *GM;
    string step2 = osCurrentFieldState.str();

    EXPECT_TRUE(step1 == state1);
    EXPECT_TRUE(step2 == state2);
}

TEST_F(GameMasterFixture, isBackWorkingCorrectly_STEP){
    ostringstream osState1;
    ostringstream osState2;
    ostringstream osCurrentFieldState;
    // FOR SET
    GM->setCell(5, 6);  // *
    GM->setCell(6, 6);  // *
    GM->setCell(7, 6);  // *
    osState1 << *GM;
    string state1 = osState1.str();

    GM->resetGame();

    GM->setCell(6, 6); //
    GM->setCell(6, 5); // ***
    GM->setCell(6, 7); //
    osState2 << *GM;
    string state2 = osState2.str();

    GM->step(1);
    osCurrentFieldState << *GM;
    string step1 = osCurrentFieldState.str();
    osCurrentFieldState.str("");

    GM->back();
    osCurrentFieldState << *GM;
    string back = osCurrentFieldState.str();

    EXPECT_TRUE(step1 == state1);
    EXPECT_TRUE(back == state2);
}

TEST_F(GameMasterFixture, isBackWorkingCorrectly_SET){
    ostringstream osState1;
    ostringstream osCurrentFieldState;
    // FOR SET
    GM->setCell(5, 6);  // *
    osState1 << *GM;
    string state1 = osState1.str();

    GM->setCell(6, 6);  // *

    GM->back();
    osCurrentFieldState << *GM;
    string back = osCurrentFieldState.str();
    osCurrentFieldState.str("");

    EXPECT_TRUE(state1 == back);
}

TEST_F(GameMasterFixture, isLoadWorkingCorrectly){
    ostringstream os1;
    ostringstream os2;
    prsr.loadGameFromFile(GM, "opa.txt");

    os1 << *GM;

    GM->resetGame();

    GM->setCell(0, 8);
    GM->setCell(1, 8);
    GM->setCell(2, 8);

    os2 << *GM;

    string out1 = os1.str();
    string out2 = os2.str();

    EXPECT_TRUE(out1 == out2);
}

TEST_F(GameMasterFixture, isSaveWorkingCorrectly){
    ostringstream os1;
    ostringstream os2;

    GM->setCell(0, 8);
    GM->setCell(1, 8);
    GM->setCell(2, 8);

    os1 << *GM;

    prsr.saveGameIntoFile(GM->getCurrentField(), "saving.txt");

    GM->resetGame();

    prsr.loadGameFromFile(GM, "saving.txt");

    os2 << *GM;

    string out1 = os1.str();
    string out2 = os2.str();

    EXPECT_TRUE(out1 == out2);
}


