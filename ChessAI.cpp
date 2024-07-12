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
#include <algorithm>
#include <stdlib.h>
#include <time.h>

#include "Board.h"
#include "ChessAI.h"
#include "ChessTypes.h"
#include "EvaluationTypes.h"
#include "Logger.h"
#include "Piece.h"

// Default Constructor
ChessAI::ChessAI(Colors colorIn, int depthIn)
    : DEPTH(depthIn),
      nodesSearched(0),
      aiPlayer(colorIn),
      board(0)
{
    srand(time(NULL));
}

    // Default Destructor
ChessAI::~ChessAI()
{

}

// @purpose: Sets the depth of the AI
// @parameters: Depth of AI
// @return: None
void ChessAI::setDepth(int strength)
{
    DEPTH = strength;
}

// @purpose: Get the Best move for a given color
// @parameters: Board object
// @return: The best move to make
Move ChessAI::getBestMove(Board* boardIn, Colors color)
{
    // Store the pointer to the board
    board = boardIn;

    // Store the best possible move
    pvNode bestPath;
    pvNode tmpNode;

    // Initial Alpha and Beta Values
    int alpha = INT_MIN;
    int beta = INT_MAX;

    // Print out the evaluation of the current position
    Logger::log("Current Eval: %i\n", evaluateBoard());

    // Disable all print Statements while searching
    Logger::disable();

    // Time the execution
    time_t start;
    time_t stop;
    time(&start);

    // Evaluate for the Maximizing Player (White)
    if (color == WHITE)
    {
        int maxEvaluation = INT_MIN;
        int currentEvaluation = INT_MIN;
        // Get all of Whites moves
        mvVector allMoves = *board->getAllMoves(WHITE);
        moveOrdering(&allMoves);

        // Loop through all possible moves for White
        for (mvVector::iterator it = allMoves.begin(); it != allMoves.end(); ++it)
        {
            // Make a candiate move
            board->makeMove(*it);

            // evaluate the results of that move
            tmpNode = miniMax(board->getMoveHistory()->back(), DEPTH - 1, alpha, beta, BLACK);
            currentEvaluation = tmpNode.eval;
            // Undo the move
            board->undoMove();

            if (currentEvaluation > maxEvaluation)
            {
                bestPath.eval = currentEvaluation;
                bestPath.variation = tmpNode.variation;

                maxEvaluation = currentEvaluation;
            }
            if (currentEvaluation > alpha)
            {
                alpha = currentEvaluation;
            }
            if (beta <= alpha)
            {
                break;
            }
        }
    }
    // Evaluate the Minimizing Player (Black)
    else
    {
        int minEvaluation = INT_MAX;
        int currentEvaluation = INT_MAX;
        // Get all of Blacks moves
        mvVector allMoves = *board->getAllMoves(BLACK);
        moveOrdering(&allMoves);

        // Loop through all possible moves for Black
        for (mvVector::iterator it = allMoves.begin(); it != allMoves.end(); ++it)
        {
            // Make a candiate move
            board->makeMove(*it);

            // evaluate the results of that move
            tmpNode = miniMax(board->getMoveHistory()->back(), DEPTH - 1, alpha, beta, WHITE);
            currentEvaluation = tmpNode.eval;

            // Undo the move
            board->undoMove();

            if (currentEvaluation < minEvaluation)
            {
                bestPath.eval = currentEvaluation;
                bestPath.variation = tmpNode.variation;

                minEvaluation = currentEvaluation;
            }
            if (currentEvaluation < beta)
            {
                beta = currentEvaluation;
            }
            if (beta <= alpha)
            {
                break;
            }
        }
    }

    // Return the Best move
    time(&stop);
    int execution = difftime(stop, start);
    int mins = execution / 60;
    int secs = execution % 60;

    // Re-enable the Logger and print best move and PV information
    Logger::enable();
    Logger::log("Searched %i Nodes in %i min %i sec\n", nodesSearched, mins, secs);
    Logger::log("The best move I found was ");
    bestPath.variation.front().printMove(true);
    Logger::log("The principal variation I found was ");
    if (color == BLACK)
    {
        Logger::log(" ..");
    }
    lastPV.clear();
    
    // Traverse the Principal Variation
    for (mvVector::iterator it = bestPath.variation.begin();
        it != bestPath.variation.end();
        ++it)
    {
        it->printMove(false);

        // Save the PV for move ordering next turn
        lastPV.push_back(*it);
    }
    Logger::log("\n\n");

    // Reset all turn based variables
    nodesSearched = 0;

    // Return the best move that was found
    return bestPath.variation.front();
}

// @purpose: Calculate the move score recursively using the MiniMax algorithm with AlphaBeta Pruning
// @parameters: int depth to search, Color to represent Min(Black) and Max(White)
// @return: score of board after N moves
ChessAI::pvNode ChessAI::miniMax(Move moveBefore, int depth, int alpha, int beta, Colors color)
{
    pvNode myNode;

    // Track how many nodes are being searched
    ++nodesSearched;

    // If we have reached a terminal node, evaluate the board
    if (depth <= 0 || board->getGameStatus() != IN_PROGRESS)
    {
        myNode.eval = evaluateBoard();
        myNode.variation.push_back(moveBefore);
        return myNode;
    }

    pvNode tmpNode;

    // Evaluate for the Maximizing Player (White)
    if (color == WHITE)
    {
        int maxEvaluation = INT_MIN;
        int currentEvaluation = INT_MIN;
        // Get all of Whites moves
        mvVector allMoves = *board->getAllMoves(WHITE);
        moveOrdering(&allMoves);

        // Loop through all possible moves for White
        for (mvVector::iterator it = allMoves.begin(); it != allMoves.end(); ++it)
        {
            // Make a candiate move
            board->makeMove(*it);

            // evaluate the results of that move
            tmpNode = miniMax(board->getMoveHistory()->back(), depth - 1, alpha, beta, BLACK);
            currentEvaluation = tmpNode.eval;

            // Undo the move
            board->undoMove();

            if (currentEvaluation > maxEvaluation)
            {
                myNode.eval = currentEvaluation;
                myNode.variation.clear();
                myNode.variation.push_back(moveBefore);
                myNode.variation.insert(myNode.variation.end(),
                    tmpNode.variation.begin(),
                    tmpNode.variation.end());

                maxEvaluation = currentEvaluation;
            }
            if (currentEvaluation > alpha)
            {
                alpha = currentEvaluation;
            }
            if (beta <= alpha)
            {
                break;
            }
        }
        return myNode;
    }
    // Evaluate the Minimizing Player (Black)
    else
    {
        int minEvaluation = INT_MAX;
        int currentEvaluation = INT_MAX;
        // Get all of Whites moves
        mvVector allMoves = *board->getAllMoves(BLACK);
        moveOrdering(&allMoves);

        // Loop through all possible moves for Black
        for (mvVector::iterator it = allMoves.begin(); it != allMoves.end(); ++it)
        {
            // Make a candiate move
            board->makeMove(*it);

            // evaluate the results of that move
            tmpNode = miniMax(board->getMoveHistory()->back(), depth - 1, alpha, beta, WHITE);
            currentEvaluation = tmpNode.eval;

            // Undo the move
            board->undoMove();

            if (currentEvaluation < minEvaluation)
            {
                myNode.eval = currentEvaluation;
                myNode.variation.clear();
                myNode.variation.push_back(moveBefore);
                myNode.variation.insert(myNode.variation.end(), 
                    tmpNode.variation.begin(), 
                    tmpNode.variation.end());

                minEvaluation = currentEvaluation;
            }
            if (currentEvaluation < beta)
            {
                beta = currentEvaluation;
            }
            if (beta <= alpha)
            {
                break;
            }
        }
        return myNode;
    }
}

// @purpose: Calculate the move score recursively using the MiniMax algorithm with AlphaBeta Pruning
// @parameters: int depth to search, Color to represent Min(Black) and Max(White)
// @return: score of board after N moves
int ChessAI::miniMax(int depth, int alpha, int beta, Colors color)
{
    // Track how many nodes are being searched
    ++nodesSearched;

    // Check if the game is over
    GameResults result = board->getGameStatus();

    // If we have reached a terminal node, evaluate the board
    if (depth <= 0 || result != IN_PROGRESS)
    {
        return evaluateBoard();
    }

    // Evaluate for the Maximizing Player (White)
    if (color == WHITE)
    {
        int maxEvaluation = INT_MIN;
        int currentEvaluation = INT_MIN;
        // Get all of Whites moves
        mvVector allMoves = *board->getAllMoves(WHITE);
        moveOrdering(&allMoves);

        // Loop through all possible moves for White
        for (mvVector::iterator it = allMoves.begin(); it != allMoves.end(); ++it)
        {
            // Make a candiate move
            board->makeMove(*it);
            Move lastmove = board->getMoveHistory()->back();

            // evaluate the results of that move
            currentEvaluation = miniMax(depth - 1, alpha, beta, BLACK);
            // Undo the move
            board->undoMove();

            if (currentEvaluation > maxEvaluation)
            {
                maxEvaluation = currentEvaluation;
            }
            if (currentEvaluation > alpha)
            {
                alpha = currentEvaluation;
            }
            if (beta <= alpha)
            {
                break;
            }
        }
        return maxEvaluation;
    }
    // Evaluate the Minimizing Player (Black)
    else
    {
        int minEvaluation = INT_MAX;
        int currentEvaluation = INT_MAX;
        // Get all of Whites moves
        mvVector allMoves = *board->getAllMoves(BLACK);
        moveOrdering(&allMoves);

        // Loop through all possible moves for White
        for (mvVector::iterator it = allMoves.begin(); it != allMoves.end(); ++it)
        {
            // Make a candiate move
            board->makeMove(*it);
            Move lastmove = board->getMoveHistory()->back();

            // evaluate the results of that move
            currentEvaluation = miniMax(depth - 1, alpha, beta, WHITE);
            // Undo the move
            board->undoMove();

            if (currentEvaluation < minEvaluation)
            {
                minEvaluation = currentEvaluation;
            }
            if (currentEvaluation < beta)
            {
                beta = currentEvaluation;
            }
            if (beta <= alpha)
            {
                break;
            }
        }
        return minEvaluation;
    }
}

// @purpose: Orders the moves for a best-first search
// @parameters: List of moves to order
// @return: None
void ChessAI::moveOrdering(mvVector* moves)
{
    if (moves->empty()) return;

    Colors activeColor = moves->front().pieceMoved->getColor();

    // Search the most advanced pieces first
    // If the pieces are White, search from the top of the board down
    // Moves are currently generated from A8 -> H1 so no change needed for white

    // If the pieces are Black, search from the bottom of the board up
    if (activeColor == BLACK)
    {
        std::reverse(moves->begin(), moves->end());
    }

    // Prioritize moves that were in the last PV
    mvVector pvMoves;
    for (mvVector::iterator it = lastPV.begin(); it != lastPV.end(); ++it)
    {
        mvVector::iterator jt = std::find(moves->begin(), moves->end(), *it);
        if (jt != moves->end())
        {
            pvMoves.push_back(*jt);
            moves->erase(jt);
        }
    }

    moves->insert(moves->begin(), pvMoves.begin(), pvMoves.end());

}

// @purpose: Calculate the score for a given board
// @parameters: None
// @return: score for the position
int ChessAI::evaluateBoard()
{
    int score = 0;

    // realtive scores - return Negative for black, positive for white
    score = calculateMaterialScore();
    score += calculateProtectionScore();
    score += calculateCheckScore();
    score += calculateEndgameScore();

    return score;
}

// @purpose: Calculate the Material Score for a position
// @parameters: None
// @return: material score for the position
int ChessAI::calculateMaterialScore()
{
    int score = 0;
    Piece* piece = 0;

    for (int square = A8; square <= H1; ++square)
    {
        piece = board->getPieceAt((Squares)square);

        if (piece == 0) continue;

        // Calcaulte the Score of the piece by the piece score, +/- the PSQT modifier
        // times the modifier per color (1 for White, -1 for Black)
        score += ((PieceScores[piece->getType()] + getPsqtModifier(piece))
                  * ScoreModifier[piece->getColor()]);
    }

    return score;
}

// @purpose: Get the PIece Square Table Modifier for a given piece
// @parameters: Pice object to evaluate
// @return: PSQT Modifier (addition)
int ChessAI::getPsqtModifier(Piece* piece)
{
    switch (piece->getType())
    {
    case PAWN:
        return PAWN_PSQT[piece->getColor()][piece->getPos()];

    case KNIGHT:
        return KNIGHT_PSQT[piece->getColor()][piece->getPos()];

    case BISHOP:
        return BISHOP_PSQT[piece->getColor()][piece->getPos()];

    case ROOK:
        return ROOK_PSQT[piece->getColor()][piece->getPos()];

    case QUEEN:
        return QUEEN_PSQT[piece->getColor()][piece->getPos()];

    case KING:
        if (isEndgame())
        {
            return KING_EG_PSQT[piece->getColor()][piece->getPos()];
        }
        else
        {
            return KING_MG_PSQT[piece->getColor()][piece->getPos()];
        }

    default:
        return 0;
    }
}

// @purpose: Checks if the board is in the endgame
// @parameters: None
// @return: true if endgame, false otherwise
bool ChessAI::isEndgame()
{
    // Currently, the definition of 'EndGame' is the absence of queens
    
    // Check if there are no more queens on the board
    for (int i = 0; i < Chess::NumSquares; ++i)
    {
        if (board->getCharBoard()[i] == 'Q' ||
            board->getCharBoard()[i] == 'q')
        {
            return false;
        }
    }

    return true;
}

// @purpose: Calculate the Mobility Score for a position
// @parameters: None
// @return: Mobility score for the position
int ChessAI::calculateMobilityScore()
{
    int whiteMobility = board->getAllMoves(WHITE)->size();
    int blackMobility = board->getAllMoves(BLACK)->size();

    return (whiteMobility - blackMobility) * MOBILITY_SCORE;
}

// @purpose: Calculate the Protection Score for a position
// @parameters: None
// @return: Protection score for the position
int ChessAI::calculateProtectionScore()
{
    // TODO - Check for friendly pieces in the move list
    return 0;
}

// @purpose: Calculate the Check Score for a position
// @parameters: None
// @return: Check score for the position
int ChessAI::calculateCheckScore()
{

    // Check if the Black King is check
    if (!board->getCheckPaths(BLACK)->empty())
    {
        // If Black is in check, that is good for white
        return CHECK_SCORE;
    }

    // Check if the White King is in check
    if (!board->getCheckPaths(WHITE)->empty())
    {
        // If White is in check, that is bad for white
        return -CHECK_SCORE;
    }

    return 0;
}

// @purpose: Calculate the Endgame Score for a position
// @parameters: None
// @return: Endgame score for the position
int ChessAI::calculateEndgameScore()
{
    GameResults result = board->getGameStatus();
    // If no endgame condition, then no effect on score
    if (result == IN_PROGRESS) return 0;

    if (result == WHITE_WIN)
    {
        return WIN_SCORE;
    }

    if (result == BLACK_WIN)
    {
        return -WIN_SCORE;
    }

    // In order to attempt to avoid draws, return a modified value based on who's turn it is
    // Return a negative draw score for white 
    // Return a positive draw score for black
    return DRAW_SCORE * ScoreModifier[board->getFen()->activeColor];
}