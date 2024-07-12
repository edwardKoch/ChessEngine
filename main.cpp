//----------------------------------------------
//
// File: main.cpp
// Author: Edward Koch
// Description: Main file to start a chess game
//
// Revision History:
// Author       Date        Description
//----------------------------------------------
// E. Koch      03/03/23    Initial Development
// 
//----------------------------------------------
#include <iostream>

#include "Board.h"
#include "Game.h"
#include "Logger.h"
#include "PerfTest.h"

int main()
{
    // Run performance Tests - https://www.chessprogramming.org/Perft_Results
    //PerfTest::runAllPerft();
    //return 0;

    //const char* fen = "8/2p5/3p3r/KP6/5p1k/8/4P1P1/R7 b - - 3 2";
    //PerfTest::runPerft(fen, 2);

    // Play Chess
    Game game;
    //game.setFen(fen);
    game.play();
    return 0;
}