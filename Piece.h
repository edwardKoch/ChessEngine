//-------------------------------------------------------
//
// File: Piece.h
// Author: Edward Koch
// Description: Holds the declaration of the Piece Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    03/03/23    Initial Creation of Piece class
//-------------------------------------------------------
#ifndef PIECE_H
#define PIECE_H

#include "ChessTypes.h"
#include "Board.h"
#include "Move.h"

class Piece
{
public:
    // Default Constructor
    Piece(char idIn, Colors colorIn, Squares posIn);

    // Default Destructor
    ~Piece();

    // @purpose: Get a character representation of this piece
    // @parameters: None
    // @return: character of this piece
    char getCharId();

    // @purpose: Get the type of this piece
    // @parameters: None
    // @return: Type of this piece
    PieceTypes getType();

    // @purpose: Get the color of this piece
    // @parameters: None
    // @return: Colors color of the piece
    Colors getColor();

    // @purpose: Get the position of this piece
    // @parameters: None
    // @return: Sqaures position of the piece
    Squares getPos();

    // @purpose: Sets the hasMoved flag (used in undoing moves)
    // @parameters: boolean if the piece has moved or not
    // @return: None
    void setHasMoved(bool moved);

    // @purpose: Calculate all possible moves given a character representation of the boad
    // @parameters: Character Representation of the board, Color of piece that is moving, Color of piece that is moving
    // @return: None
    virtual void calculateMoves(Board* board, Colors turn) = 0;

    // @purpose: Get the move vector of this piece
    // @parameters: None
    // @return: Vector of moves
    mvVector* getMoves();

    // @purpose: Checks if an end square is in this pieces set of moves
    // @parameters: Square to move to
    // @return: True if it is a valid ending square, false otherwise
    bool isSquareInMoves(Squares sq);

    // @purpose: Get the squares that this piece is protecting
    // @parameters: None
    // @return: Vector of squares
    sqVector* getProtecting();

    // @purpose: Sets the pinnedPath of this piece
    // @parameters: Vector of squares
    // @return: None
    void setPin(sqVector pinPath);

    // @purpose: Clears the pinnedPath of this piece between each move
    // @parameters: None
    // @return: None
    void clearPin();

    // @purpose: Make a move to a new position
    // @parameters: New Square to move to
    // @return: None
    void makeMove(Squares newPos);

    // @purpose: Prints all possible moves for this piece
    // @parameters: None
    // @return: None
    void printMoves();

    // @purpose: Print the Piece details
    // @parameters: None
    // @return: None
    void printPiece();

protected:
    // Character representaiton of a piece
    char charId;

    PieceTypes type;

    // Holds the piece color (WHITE or BLACK)
    Colors color;

    // Determines if this piece has moved yet
    bool hasMoved;

    // Holds the position of the piece
    Squares position;

    // Holds all current possible moves 
    mvVector moves;
    sqVector protecting;

    // Holds the path of an attacking piece to the friendly king through this piece(if one exists)
    sqVector pinnedPath;

    // @purpose: Checks if a move would take this piece out of a potential pin situation
    // @parameters: Square to move to
    // @return: true if square is in the pin path, false otherwise
    bool isLegalMove(Board* board, Squares potentialMove);

    // @purpose: Creates a Move object and saves it to the potential moves
    // @parameters: Square index to move to
    // @return: None
    void addMove(int potentialMove);

private:


};

#endif // PIECE_H

