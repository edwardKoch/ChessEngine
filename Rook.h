//-------------------------------------------------------
//
// File: Piece.h
// Author: Edward Koch
// Description: Holds the definition of the Piece Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    03/04/23    Initial Creation of Rook class
//-------------------------------------------------------
#ifndef ROOK_H
#define ROOK_H

#include "SlidingPiece.h"

class Rook : public SlidingPiece
{
public:

    // Default Constructor
    Rook(Colors colorIn, Squares posIn);

    // Default Destructor
    ~Rook();

    // @purpose: Calculate all possible moves given a character representation of the boad
    // @parameters: Character Representation of the board, Color of piece that is moving
    // @return: None
    virtual void calculateMoves(Board* board, Colors turn);

private:

};

#endif //ROOK_H