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
#ifndef BISHOP_H
#define BISHOP_H

#include "SlidingPiece.h"

class Bishop : public SlidingPiece
{
public:

    // Default Constructor
    Bishop(Colors colorIn, Squares posIn);

    // Default Destructor
    ~Bishop();

    // @purpose: Calculate all possible moves given a character representation of the boad
    // @parameters: Character Representation of the board, Color of piece that is moving
    // @return: None
    virtual void calculateMoves(Board* board, Colors turn);

private:

};

#endif //BISHOP_H