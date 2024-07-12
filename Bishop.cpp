//-------------------------------------------------------
//
// File: Piece.h
// Author: Edward Koch
// Description: Holds the definition of the Piece Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    03/04/23    Initial Creation of Bishop class
//-------------------------------------------------------
#include <cctype>

#include "Bishop.h"

// Default Constructor
Bishop::Bishop(Colors colorIn, Squares posIn)
    : SlidingPiece('b', colorIn, posIn)
{
    type = BISHOP;
}

// Default Destructor
Bishop::~Bishop()
{

}

// @purpose: Calculate all possible moves given a character representation of the boad
// @parameters: Character Representation of the board, Color of piece that is moving
// @return: None
void Bishop::calculateMoves(Board* board, Colors turn)
{
    // Clear the current move vector
    moves.clear();
    protecting.clear();

    // Calculate moves in all possible directions
    calculateSlidingMoves(board, UP_LEFT);
    calculateSlidingMoves(board, UP_RIGHT);
    calculateSlidingMoves(board, DN_LEFT);
    calculateSlidingMoves(board, DN_RIGHT);

}



