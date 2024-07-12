//-------------------------------------------------------
//
// File: PerfTest.cpp
// Author: Edward Koch
// Description: Holds the definition of the PerfTest Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    04/16/23    Initial Creation of PerfTest class
//-------------------------------------------------------
#include <cctype>
#include <time.h>

#include "Board.h"
#include "Logger.h"
#include "PerfTest.h"
#include "Piece.h"

Board* PerfTest::board = 0;

// @purpose: Runs the PerfT function on all test fen to depth 3 - https://www.chessprogramming.org/Perft
// @parameters: None
// @return: None
void PerfTest::runAllPerft()
{
    int totalNodes = 0;
    int totalTime = 0;

    const int NUM_POS = 6;
    const char* fens[NUM_POS] =
    {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", // Position 1
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", // Position 2
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", // Position 3
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", // Position 4
        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", // Position 5
        "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10" // Position 6
    };

    // Expected Results from  - https://www.chessprogramming.org/Perft
    const int DEPTH = 5;
    const int results[NUM_POS][DEPTH] =
    {
        {20, 400,  8902,  197281,  4865609},  // Position 1 depths 1, 2, 3, 4, 5
        {48, 2039, 97862, 4085603, 193690690},// Position 2 depths 1, 2, 3, 4, 5
        {14, 191,  2812,  43238,   674624},   // Position 3 depths 1, 2, 3, 4, 5
        {6,  264,  9467,  422333,  15833292}, // Position 4 depths 1, 2, 3, 4, 5
        {44, 1486, 62379, 2103487, 89941194}, // Position 5 depths 1, 2, 3, 4, 5
        {46, 2079, 89890, 3894594, 164075551} // Position 6 depths 1, 2, 3, 4, 5
    };

    for (int position = 0; position < NUM_POS; ++position)
    {
        for (int depth = 0; depth < DEPTH; ++depth)
        {
            if (results[position][depth] > 10000000)
            {
                Logger::enable();
                Logger::log("Position %i at depth %i is too long\n", position + 1, depth + 1);
                Logger::restore();
                continue;
            }
            Logger::disable();
            // Time the execution
            time_t start;
            time_t stop;
            time(&start);
            int numNodes = runPerft(fens[position], depth+1);
            time(&stop);
            int execution = difftime(stop, start);
            int mins = execution / 60;
            int secs = execution % 60;
            Logger::enable();
            if (numNodes == results[position][depth])
            {
                Logger::log("Position %i at depth %i PASSED! %i Nodes in %i min %i sec\n", position + 1, depth + 1, numNodes, mins, secs);
            }
            else
            {
                Logger::log("Position %i at depth %i FAILED - %i/%i in %i min %i sec\n", position+1, depth+1, numNodes, results[position][depth], mins, secs);
                // Run test again with output
                runPerft(fens[position], depth + 1);
                Logger::log("\n\n");
            }

            totalNodes += numNodes;
            totalTime += execution;
        }
    }
    Logger::enable();
    Logger::log("Average Nodes/Second = %d\n", totalNodes / totalTime);
    Logger::restore();
}

// @purpose: Runs the PerfT function on a given fen
// @parameters: fen string, depth to run
// @return: Number of Nodes
int PerfTest::runPerft(const char* fen, int depth)
{
    Board newBoard(fen);
    board = &newBoard;

    Colors startingColor = board->getFen()->activeColor;

    int depthNodes = 0;

    // Time the execution
    time_t start;
    time_t stop;
    time(&start);

    // Get all of Whites moves
    mvVector allMoves = *board->getAllMoves(startingColor);

    // Loop through all possible moves for White
    for (mvVector::iterator it = allMoves.begin(); it != allMoves.end(); ++it)
    {
        // Make a candiate move
        board->makeMove(*it);

        // Count the number of sub-nodes
        int moveNodes = perft(depth - 1, Chess::enemy[startingColor]);

        // Undo the move
        board->undoMove();

        // Print the initial move
        //Logger::enable();
        //if (it->pieceMoved->getType() != PAWN)
        //{
        //    Logger::log("%c", toupper(it->pieceMoved->getCharId()));
        //}
        Logger::log("%c%c", tolower(Chess::File[it->start % Chess::NumFiles]),
            Chess::Rank[it->start / Chess::NumRanks]);
        Logger::log("%c%c", tolower(Chess::File[it->end % Chess::NumFiles]),
            Chess::Rank[it->end / Chess::NumRanks]);

        // Print the number of nodes that came from that first move
        Logger::log(": %i\n", moveNodes);

        depthNodes += moveNodes;
    }

    time(&stop);
    int execution = difftime(stop, start);
    int mins = execution / 60;
    int secs = execution % 60;

    //Logger::enable();
    Logger::log("%i Nodes at Depth %i in% i min% i sec\n", depthNodes, depth, mins, secs);

    return depthNodes;
}

// @purpose: Counts the nodes at a given depth - used to test move generation
// @parameters: int depth, color to search for
// @return: None
int PerfTest::perft(int depth, Colors color)
{
    if (depth == 0)
    {
        return 1;
    }

    // Track the number of nodes
    int nodes = 0;

    // Get all of Whites moves
    mvVector allMoves = *board->getAllMoves(color);

    // Loop through all possible moves for White
    for (mvVector::iterator it = allMoves.begin(); it != allMoves.end(); ++it)
    {
        // Make a candiate move
        board->makeMove(*it);
        // Count the number of sub-nodes
        nodes += perft(depth - 1, Chess::enemy[color]);

        // Undo the move
        board->undoMove();
    }

    return nodes;
}

// @purpose: Times move genreation and move making to determine efficiency
// @parameters: None
// @return: None
void PerfTest::timeT()
{

}



