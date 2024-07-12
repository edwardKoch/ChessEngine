//-------------------------------------------------------
//
// File: PerfTest.h
// Author: Edward Koch
// Description: Holds the declaration of the PerfTest Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    04/16/23    Initial Creation of PerfTest class
//-------------------------------------------------------
#ifndef PERF_TEST_H
#define PERF_TEST_H

// Forward Declaration
class Board;

class PerfTest
{
public:

    // @purpose: Runs the PerfT function on all test fen to depth 3 - https://www.chessprogramming.org/Perft
    // @parameters: None
    // @return: None
    static void runAllPerft();

    // @purpose: Runs the PerfT function on a given fen
    // @parameters: fen string, depth to run
    // @return: Number of Nodes
    static int runPerft(const char* fen, int depth);

    // @purpose: Counts the nodes at a given depth - used to test move generation
    // @parameters: int depth, color to calculate for
    // @return: Number of Nodes
    static int perft(int depth, Colors color);

    // @purpose: Times move genreation and move making to determine efficiency
    // @parameters: None
    // @return: None
    static void timeT();

private:
    // Pointer to a Board for use in perft
    static Board* board;

    // Default Constructor
    PerfTest();

    // Default Destructor
    ~PerfTest();
};

#endif // PERF_TEST_H