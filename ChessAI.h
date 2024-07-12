//-------------------------------------------------------
//
// File: ChessAI.h
// Author: Edward Koch
// Description: Holds the declaration of the ChessAI Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    04/10/23    Initial Creation of ChessAI class
//-------------------------------------------------------
#ifndef CHESS_AI_H
#define CHESS_AI_H

#include <map>

#include "ChessTypes.h"
#include "EvaluationTypes.h"
#include "Move.h"

// Forward Declarations
class Board;
class Piece;

class ChessAI
{
public:
    // Default Constructor
    ChessAI(Colors colorIn, int depthIn);

    // Default Destructor
    ~ChessAI();

    // @purpose: Sets the depth of the AI
    // @parameters: Depth of AI
    // @return: None
    void setDepth(int strength);

    // @purpose: Get the Best move for a given color
    // @parameters: Board object
    // @return: The best move to make
    Move getBestMove(Board* boardIn, Colors color);

private:

    // Struct to track the best variation
    struct pvNode
    {
        int eval;
        mvVector variation;

        pvNode() : eval(0), variation() { ; }
    };

    // How far to Search
    int DEPTH;

    // How many Nodes were searched
    int nodesSearched;

    // What Color the AI is playing
    Colors aiPlayer;

    // Board that the AI is playing on
    Board* board;

    // Stores the PV from last search
    mvVector lastPV;

    // @purpose: Calculate the move score recursively using the MiniMax algorithm with AlphaBeta Pruning
    // @parameters: int depth to search, Color to represent Min(Black) and Max(White)
    // @return: Node of the best move and it's evaluation
    ChessAI::pvNode miniMax(Move moveBefore, int depth, int alpha, int beta, Colors color);

    // @purpose: Calculate the move score recursively using the MiniMax algorithm with AlphaBeta Pruning
    // @parameters: int depth to search, Color to represent Min(Black) and Max(White)
    // @return: Node of the best move and it's evaluation
    int miniMax(int depth, int alpha, int beta, Colors color);

    // @purpose: Orders the moves for a best-first search
    // @parameters: List of moves to order
    // @return: None
    void moveOrdering(mvVector* moves);

    // @purpose: Calculate the score for a given board
    // @parameters: None
    // @return: score for the position
    int evaluateBoard();

    // @purpose: Calculate the Material Score for a position
    // @parameters: None
    // @return: material score for the position
    int calculateMaterialScore();

    // @purpose: Get the PIece Square Table Modifier for a given piece
    // @parameters: Pice object to evaluate
    // @return: PSQT Modifier (addition)
    int getPsqtModifier(Piece* piece);

    // @purpose: Checks if the board is in the endgame
    // @parameters: None
    // @return: true if endgame, false otherwise
    bool isEndgame();

    // @purpose: Calculate the Mobility Score for a position
    // @parameters: None
    // @return: Mobility score for the position
    int calculateMobilityScore();

    // @purpose: Calculate the Protection Score for a position
    // @parameters: None
    // @return: Protection score for the position
    int calculateProtectionScore();

    // @purpose: Calculate the Check Score for a position
    // @parameters: None
    // @return: Check score for the position
    int calculateCheckScore();

    // @purpose: Calculate the Endgame Score for a position
    // @parameters: None
    // @return: Endgame score for the position
    int calculateEndgameScore();


};

#endif // CHESS_AI_H