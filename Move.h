//-------------------------------------------------------
//
// File: Move.h
// Author: Edward Koch
// Description: Holds the declaration of the Move Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    04/08/23    Initial Creation of Move class
//-------------------------------------------------------
#ifndef MOVE_H
#define MOVE_H

#include "ChessTypes.h"
#include "Fen.h"

// Forward Declaration
class Piece;

class Move
{
public:
    // Default Constructor
    Move();

    // Default Destructor
    ~Move();

    // Standard Move Information
    Piece* pieceMoved;
    Squares start;
    Squares end;
    char rankDisambig;
    char fileDisambig;

    // Non-Reversible Move Information
    Piece* pieceCaptured;
    bool isEnPassant;
    Piece* pieceCastled;
    Squares castledFrom;
    Piece* piecePromoted;
    char targetPromotion;

    // Board Information
    Fen oldFen;

    // @purpose: Prints the move in algebraic notation
    // @parameters: None
    // @return: None
    void printMove(bool newline);

    // @purpose: Prints the move in algebraic notation
    // @parameters: None
    // @return: None
    void printDebug(bool newline);

    // Comparator function
    bool operator == (const Move& m);

private:

};

typedef std::vector<Move> mvVector;

#endif // MOVE_H