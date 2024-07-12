//-------------------------------------------------------
//
// File: Piece.h
// Author: Edward Koch
// Description: Holds the definition of the Piece Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    03/04/23    Initial Creation of Queen class
//-------------------------------------------------------
#include <cctype>

#include "Queen.h"

// Default Constructor
Queen::Queen(Colors colorIn, Squares posIn)
    : SlidingPiece('q', colorIn, posIn)
{
    type = QUEEN;
}

// Default Destructor
Queen::~Queen()
{

}

// @purpose: Calculate all possible moves given a character representation of the boad
// @parameters: Character Representation of the board, Color of piece that is moving
// @return: None
void Queen::calculateMoves(Board* board, Colors turn)
{
    // Clear the current move vector
    moves.clear();
    protecting.clear();

    // Calculate moves in all possible directions
    calculateSlidingMoves(board, UP);
    calculateSlidingMoves(board, DOWN);
    calculateSlidingMoves(board, LEFT);
    calculateSlidingMoves(board, RIGHT);
    calculateSlidingMoves(board, UP_LEFT);
    calculateSlidingMoves(board, UP_RIGHT);
    calculateSlidingMoves(board, DN_LEFT);
    calculateSlidingMoves(board, DN_RIGHT);

}

