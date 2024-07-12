//-------------------------------------------------------
//
// File: Piece.h
// Author: Edward Koch
// Description: Holds the definition of the Piece Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    03/04/23    Initial Creation of Pawn class
//-------------------------------------------------------
#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Pawn : public Piece
{
public:

    // Default Constructor
    Pawn(Colors colorIn, Squares posIn);

    // Default Destructor
    ~Pawn();

    // @purpose: Calculate all possible moves given a character representation of the boad
    // @parameters: Character Representation of the board, Color of piece that is moving
    // @return: None
    virtual void calculateMoves(Board* board, Colors turn);

private:
    // @purpose: Check if En Passant will cause a check
    // @parameters: board index to move to, board object to search
    // @return: None
    bool safeEnPassant(int end, Board* board);

    // @purpose: Add All possible moves to the move list - including opssible promotion
    // @parameters: Square to move to
    // @return: None
    void addPawnMoves(int end);

};

#endif //PAWN_H