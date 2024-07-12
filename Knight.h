//-------------------------------------------------------
//
// File: Piece.h
// Author: Edward Koch
// Description: Holds the definition of the Piece Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    03/04/23    Initial Creation of Knight class
//-------------------------------------------------------
#ifndef KNIGHT_H
#define KNIGHT_H


#include "Piece.h"

class Knight : public Piece
{
public:

    // Default Constructor
    Knight(Colors colorIn, Squares posIn);

    // Default Destructor
    ~Knight();

    // @purpose: Calculate all possible moves given a character representation of the boad
    // @parameters: Character Representation of the board, Color of piece that is moving
    // @return: None
    virtual void calculateMoves(Board* board, Colors turn);

private:

    // @purpose: Check if any other of the same piece could move to the same square
    // @parameters: Char board to search, Move to update
    // @return: None
    void addDisambiguations(const char* charBoard, Move& move);

};

#endif //KNIGHT_H