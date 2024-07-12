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
#include <cctype>

#include "King.h"

// Default Constructor
King::King(Colors colorIn, Squares posIn)
    : Piece('k', colorIn, posIn)
{
    type = KING;
}

// Default Destructor
King::~King()
{

}

// @purpose: Calculate all possible moves given a character representation of the boad
// @parameters: Character Representation of the board, Color of piece that is moving
// @return: None
void King::calculateMoves(Board* board, Colors turn)
{
    // Clear all possible moves
    moves.clear();
    protecting.clear();

    // Get Character Representation of the board
    const char* charBoard = board->getCharBoard();

    // Holds all possible differences between current position and desired square
    int moveValues[8] = { -9, -8, -7, -1, 1, 7, 8, 9 };

    // All of the squares that the enemy pieces are protecting
    sqVector* enemyProtecting;
    if (turn == WHITE)
    {
        enemyProtecting = board->getAllProtected(BLACK);
    }
    else
    {
        enemyProtecting = board->getAllProtected(WHITE);
    }

    // Calculate all of the Kings moves
    for (int i = 0; i < 8; i++)
    {
        int potentialMove = position + moveValues[i];

        // Do not check moves that are out of range
        if (potentialMove < A8 || potentialMove > H1)
        {
            continue;
        }

        // Avoid wrapping around the board
        if (abs(position % Chess::NumFiles - potentialMove % Chess::NumFiles) > 1)
        {
            // If the files are furthur than 2 apart, there was wrapping
            continue;
        }

        // If the square is empty or is an enemy piece
        if (charBoard[potentialMove] == '-' ||
            (charBoard[potentialMove] != '-' &&
                isupper(charBoard[potentialMove]) != isupper(charId)))
        {
            // Check if the move would put the King in Check
            sqVector::iterator it = std::find(enemyProtecting->begin(), enemyProtecting->end(), potentialMove);

            if (it == enemyProtecting->end())
            {
                // Add the potential move to the move vector
                addMove(potentialMove);
            }
            // The King is still 'protecting' this square - which disallows the enemy king to move there
            protecting.push_back((Squares)potentialMove);
        }
        // If the square has a friendly piece
        else if (charBoard[potentialMove] != '-' &&
            isupper(charBoard[potentialMove]) == isupper(charId))
        {
            protecting.push_back((Squares)potentialMove);
        }
    }

    // Handle Castleing
    // Can not castle if in check
    if (!board->getCheckPaths(color)->empty())
    {
        return;
    }
    if (color == WHITE && position == E1)
    {
        // Check Queenside Castling
        if (charBoard[D1] == '-' && charBoard[C1] == '-' && charBoard[B1] == '-' &&
            charBoard[A1] == 'R' && board->getFen()->queenCastleWhite)
        {
            // Check if the move would put the King in Check
            sqVector::iterator it = std::find(enemyProtecting->begin(), enemyProtecting->end(), D1);
            sqVector::iterator jt = std::find(enemyProtecting->begin(), enemyProtecting->end(), C1);

            if (it == enemyProtecting->end() &&
                jt == enemyProtecting->end())
            {
                // Add the potential move to the move vector
                Move move;
                move.pieceMoved = this;
                move.start = position;
                move.end = C1;
                move.pieceCastled = board->getPieceAt(A1);
                move.castledFrom = A1;
                moves.push_back(move);
            }
        }
        // Check Kingside Castling
        if (charBoard[F1] == '-' && charBoard[G1] == '-' && 
            charBoard[H1] == 'R' && board->getFen()->kingCastleWhite)
        {
            // Check if the move would put the King in Check
            sqVector::iterator it = std::find(enemyProtecting->begin(), enemyProtecting->end(), F1);
            sqVector::iterator jt = std::find(enemyProtecting->begin(), enemyProtecting->end(), G1);

            if (it == enemyProtecting->end() &&
                jt == enemyProtecting->end())
            {
                // Add the potential move to the move vector
                Move move;
                move.pieceMoved = this;
                move.start = position;
                move.end = G1;
                move.pieceCastled = board->getPieceAt(H1);
                move.castledFrom = H1;
                moves.push_back(move);
            }
        }
    }
    else if (color == BLACK && position == E8)
    {
        // Check Queenside Castling
        if (charBoard[D8] == '-' && charBoard[C8] == '-' && charBoard[B8] == '-' &&
            charBoard[A8] == 'r' && board->getFen()->queenCastleBlack)
        {
            // Check if the move would put the King in Check
            sqVector::iterator it = std::find(enemyProtecting->begin(), enemyProtecting->end(), D8);
            sqVector::iterator jt = std::find(enemyProtecting->begin(), enemyProtecting->end(), C8);

            if (it == enemyProtecting->end() &&
                jt == enemyProtecting->end())
            {
                // Add the potential move to the move vector
                Move move;
                move.pieceMoved = this;
                move.start = position;
                move.end = C8;
                move.pieceCastled = board->getPieceAt(A8);
                move.castledFrom = A8;
                moves.push_back(move);
            }
        }
        // Check Kingside Castling
        if (charBoard[F8] == '-' && charBoard[G8] == '-' && 
            charBoard[H8] == 'r' && board->getFen()->kingCastleBlack)
        {
            // Check if the move would put the King in Check
            sqVector::iterator it = std::find(enemyProtecting->begin(), enemyProtecting->end(), F8);
            sqVector::iterator jt = std::find(enemyProtecting->begin(), enemyProtecting->end(), G8);

            if (it == enemyProtecting->end() &&
                jt == enemyProtecting->end())
            {
                // Add the potential move to the move vector
                Move move;
                move.pieceMoved = this;
                move.start = position;
                move.end = G8;
                move.pieceCastled = board->getPieceAt(H8);
                move.castledFrom = H8;
                moves.push_back(move);
            }
        }
    }
}

