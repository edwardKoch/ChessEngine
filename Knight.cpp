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
#include <cctype>

#include "Knight.h"

// Default Constructor
Knight::Knight(Colors colorIn, Squares posIn)
    : Piece('n', colorIn, posIn)
{
    type = KNIGHT;
}

// Default Destructor
Knight::~Knight()
{

}

// @purpose: Calculate all possible moves given a character representation of the boad
// @parameters: Character Representation of the board, Color of piece that is moving
// @return: None
void Knight::calculateMoves(Board* board, Colors turn)
{
    // Clear all possible moves
    moves.clear();
    protecting.clear();

    // Get Character Representation of the board
    const char* charBoard = board->getCharBoard();

    // Path of the attacking piece to the King - 
    // For a knight, only the current position is in the path, as knights cant be blocked
    sqVector pathToKing;
    pathToKing.push_back(position);

    // Holds all possible differences between current position and desired square
    const int numMoves = 8;
    int moveValues[numMoves] = { -17, -15, -10, -6, 6, 10, 15, 17 };

    for (int i = 0; i < numMoves; i++)
    {
        int potentialMove = position + moveValues[i];

        // Do not check moves that are out of range
        if (potentialMove < A8 || potentialMove > H1)
        {
            continue;
        }

        // Avoid wrapping around the board
        if (abs(position % Chess::NumFiles - potentialMove % Chess::NumFiles) > 2)
        {
            // If the files are furthur than 2 apart, there was wrapping
            continue;
        }

        // Check if this would be an otherwise illegal move (Pin/Check/etc)
        if (!isLegalMove(board, (Squares)potentialMove))
        {
            continue;
        }

        // If the square is empty
        if (charBoard[potentialMove] == '-')
        {
            // Add the potential move to the move vector
            addMove(potentialMove);
            addDisambiguations(charBoard, moves.back());
            protecting.push_back((Squares)potentialMove);
        } 
        // If the square has an enemy piece
        else if (isupper(charBoard[potentialMove]) != isupper(charId))
        {
            // Add the potential move to the move vector
            addMove(potentialMove);
            addDisambiguations(charBoard, moves.back());

            // If the piece is a King, put it in check
            if (board->getPieceAt((Squares)potentialMove)->getType() == KING)
            {
                pathToKing.push_back((Squares)potentialMove);
                board->addCheckPath(pathToKing, Chess::enemy[color]);
            }
        }
        // If the square has a friendly piece
        else if (isupper(charBoard[potentialMove]) == isupper(charId))
        {
            protecting.push_back((Squares)potentialMove);
        }
    }
}

// @purpose: Check if any other of the same piece could move to the same square
// @parameters: Char board to search, Move to update
// @return: None
void Knight::addDisambiguations(const char* charBoard, Move &move)
{
    // Holds all possible differences between current position and desired square
    const int numMoves = 8;
    int moveValues[numMoves] = { -17, -15, -10, -6, 6, 10, 15, 17 };

    // Check for Disambiguations of the move
    for (int j = 0; j < numMoves; j++)
    {
        int potentialDiambiguation = move.end + moveValues[j];

        // Do not check moves that are out of range
        if (potentialDiambiguation < A8 || potentialDiambiguation > H1)
        {
            continue;
        }

        // Avoid wrapping around the board
        if (abs(move.end % Chess::NumFiles - potentialDiambiguation % Chess::NumFiles) > 2)
        {
            // If the files are furthur than 2 apart, there was wrapping
            continue;
        }

        // Do not check the move from the potential move back to where this piece is
        if (potentialDiambiguation == position)
        {
            continue;
        }

        // Only check for other Knight of the same color
        if (charBoard[potentialDiambiguation] == charId)
        {
            // Check if the other piece is on a different file
            if (potentialDiambiguation % Chess::NumFiles !=
                move.start % Chess::NumFiles)
            {
                move.fileDisambig = Chess::File[move.start % Chess::NumFiles];
            }
            // Check if the other piece is on a different rank
            else if (potentialDiambiguation / Chess::NumRanks !=
                move.start / Chess::NumRanks)
            {
                move.rankDisambig = Chess::Rank[move.start / Chess::NumRanks];
            }
        }
    }
}

