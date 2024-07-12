//-------------------------------------------------------
//
// File: SlidingPiece.h
// Author: Edward Koch
// Description: Holds the declaration of the SlidingPiece Class
//              This class simply to commonize the sliding piece 
//              move generation across Queen, Rook, and Bishop
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    04/07/23    Initial Creation of SlidingPiece class
//-------------------------------------------------------
#ifndef SLIDING_PIECE_H
#define SLIDING_PIECE_H

#include "Piece.h"

// Forward Declarations
class Board;

class SlidingPiece : public Piece
{
public:
    // Default Constructor
    SlidingPiece(char idIn, Colors colorIn, Squares posIn);

    // Default Destructor
    ~SlidingPiece();

protected:
    enum Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        UP_LEFT,
        UP_RIGHT,
        DN_LEFT,
        DN_RIGHT
    };

    // The difference in board index coresponding to each direciton
    const int directionValues[DN_RIGHT + 1] =
    {
        -8, // Up
        8,  // Down
        -1, // Left
        1,  // Right
        -9, // Up Left
        -7, // Up Right
        7,  // Down Left
        9   // Down Right
    };

    // @purpose: Calculate all possible sliding moves in one direction
    // @parameters: Character Representation of the board, direction that the piece is going
    // @return: None
    void calculateSlidingMoves(Board* board, Direction dir);

private:

    // @purpose: Get the wrap file for a given direction
    // @parameters: Direction to wrap
    // @return: File index to check against wrapping
    int getWrapFile(Direction dir);

    // @purpose: Check if any other of the same piece could move to the same square
    // @parameters: Char board to search, Move to update
    // @return: None
    void addDisambiguations(const char* charBoard, Move& move);
};

#endif // SLIDING_PIECE_H