//-------------------------------------------------------
//
// File: Piece.h
// Author: Edward Koch
// Description: Holds the definition of the Piece Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    03/04/23    Initial Creation of King class
//-------------------------------------------------------
#ifndef KING_H
#define KING_H

#include "Piece.h"

class King : public Piece
{
public:

    // Default Constructor
    King(Colors colorIn, Squares posIn);

    // Default Destructor
    ~King();

    // @purpose: Calculate all possible moves given a character representation of the boad
    // @parameters: Character Representation of the board, Color of piece that is moving
    // @return: None
    virtual void calculateMoves(Board* board, Colors turn);

private:

};

#endif //KING_H