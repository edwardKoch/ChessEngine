//-------------------------------------------------------
//
// File: Rook.cpp
// Author: Edward Koch
// Description: Holds the definition of the Rook Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    03/04/23    Initial Creation of Rook class
//-------------------------------------------------------
#include <cctype>

#include "ChessTypes.h"
#include "Rook.h"

// Default Constructor
Rook::Rook(Colors colorIn, Squares posIn)
    : SlidingPiece('r', colorIn, posIn)
{
    type = ROOK;
}

// Default Destructor
Rook::~Rook()
{

}

// @purpose: Calculate all possible moves given a character representation of the boad
// @parameters: Character Representation of the board, Color of piece that is moving
// @return: None
void Rook::calculateMoves(Board* board, Colors turn)
{
    // Clear the current move vector
    moves.clear();
    protecting.clear();

    // Calculate moves in all possible directions
    calculateSlidingMoves(board, UP);
    calculateSlidingMoves(board, DOWN);
    calculateSlidingMoves(board, LEFT);
    calculateSlidingMoves(board, RIGHT);

}


