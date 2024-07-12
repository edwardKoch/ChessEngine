//-------------------------------------------------------
//
// File: SlidingPiece.cpp
// Author: Edward Koch
// Description: Holds the definition of the SlidingPiece Class
//              This class simply to commonize the sliding piece 
//              move generation across Queen, Rook, and Bishop
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    04/07/23    Initial Creation of SlidingPiece class
//-------------------------------------------------------
#include <cctype>
#include <vector>

#include "Board.h"
#include "SlidingPiece.h"

// Default Constructor
SlidingPiece::SlidingPiece(char idIn, Colors colorIn, Squares posIn)
    : Piece(idIn, colorIn, posIn)
{
}

// Default Destructor
SlidingPiece::~SlidingPiece()
{
}

// @purpose: Calculate all possible sliding moves in one direction
// @parameters: Character Representation of the board, direction that the piece is going
// @return: None
void SlidingPiece::calculateSlidingMoves(Board* board, Direction dir)
{
    // Determines if this piece is pinning another piece
    bool potentialPin = false;
    // The square of the piece who potentially is pinned
    Squares potentialPinSquare = INVALID;
    // Path of the attacking piece to the King - 
    // From the current piece to the enemy king, potentially through a pinned piece
    sqVector pathToKing;
    pathToKing.push_back(position);

    // Get Character Representation of the board
    const char* charBoard = board->getCharBoard();

    // Determines if a move has wrapped around the board
    int wrapFile = getWrapFile(dir);

    // Loop through all moves in one direction
    for (int numSquares = 1; numSquares <= Chess::NumFiles; ++numSquares)
    {
        int potentialMove = position + (directionValues[dir] * numSquares);

        // Do not check moves that are off of the board
        if (potentialMove < A8 || potentialMove > H1)
        {
            // Stop Searching in this direction
            break;
        }
        // Do not check moves that have wrapped around the board
        else if (potentialMove % Chess::NumFiles == wrapFile)
        {
            // Stop Searching in this direction
            break;
        }
        // Check if the square is empty
        else if (charBoard[potentialMove] == '-')
        {
            // If we have not previously encountered an enemy piece
            if (!potentialPin)
            {
                // And if this move would be otherwise legal
                if (isLegalMove(board, (Squares)potentialMove))
                {
                    // Add the potential capture to the move vector
                    addMove(potentialMove);
                    addDisambiguations(charBoard, moves.back());
                }
                // Add the potential move to the protecting vector
                protecting.push_back((Squares)potentialMove);
            }
            
            // All empty squares are potentially in the path to the enemy king
            pathToKing.push_back((Squares)potentialMove);

        }
        // Check if there is a friendly Piece in the way
        else if (isupper(charBoard[potentialMove]) == isupper(charId))
        {
            // If we have not previously encountered an enemy piece
            if (!potentialPin)
            {
                // Add the potential move to the protecting vector
                protecting.push_back((Squares)potentialMove);
            }

            // Stop Searching in this direction
            break;
        }
        // Check if there is an enemy piece in the way
        else if (isupper(charBoard[potentialMove]) != isupper(charId))
        {
            // If we have not previously encountered an enemy piece
            if (!potentialPin)
            {
                // And if this move would be otherwise legal
                if (isLegalMove(board, (Squares)potentialMove))
                {
                    // Add the potential capture to the move vector
                    addMove(potentialMove);
                    addDisambiguations(charBoard, moves.back());
                }

                // If the enemy piece is not a king, it is potentially 'pinned' to the King
                if (board->getPieceAt((Squares)potentialMove)->getType() != KING)
                {
                    potentialPin = true;
                    potentialPinSquare = (Squares)potentialMove;
                }
                // If there is no pin, and the king is found, then king is in check
                else
                {
                    pathToKing.push_back((Squares)potentialMove);
                    board->addCheckPath(pathToKing, Chess::enemy[color]);
                }
            }
            // Evaluate a potential pin situation
            else
            {
                // If there is a potential pin and the piece is pinned to a king
                if (board->getPieceAt((Squares)potentialMove)->getType() == KING)
                {
                    board->getPieceAt(potentialPinSquare)->setPin(pathToKing);
                }

                // Stop Searching in this direction
                break;
            }  // !potentialPin  
        } // piece check
    } // For Loop
}

// @purpose: Get the wrap file for a given direction
// @parameters: Direction to wrap
// @return: File index to check against wrapping
int SlidingPiece::getWrapFile(Direction dir)
{
    // Determines if a move has wrapped around the board
    int wrapFile = 9; // Invalid File
    // Determines 
    switch (dir)
    {
        // Up and Down do not wrap around the board
    case UP:
    case DOWN:
        break;

    case LEFT:
    case UP_LEFT:
    case DN_LEFT:
        wrapFile = 7; // H-File - 1 for 0 based indexing
        break;

    case RIGHT:
    case UP_RIGHT:
    case DN_RIGHT:
        wrapFile = 0; // A-File - 1 for 0 based indexing
        break;

    default:
        break;
    }

    return wrapFile;
}


// @purpose: Check if any other of the same piece could move to the same square
// @parameters: Char board to search, Move to update
// @return: None
void SlidingPiece::addDisambiguations(const char* charBoard, Move& move)
{
    // Check in all directions for a potential disambiguating piece
    for (int dir = 0; dir <= DN_RIGHT; ++dir)
    {
        // Only process U, D, L, R moves for Rooks
        if (dir > RIGHT && type == ROOK) continue;

        // Only process UR, UL, DR, DL moves for Bishops
        if (dir < UP_LEFT && type == BISHOP) continue;

        // Loop through all squares in that direction
        // Determines if a move has wrapped around the board
        int wrapFile = getWrapFile((Direction)dir);

        // Check to the end of the board in each direction
        for (int numSquares = 1; numSquares <= Chess::NumFiles; ++numSquares)
        {
            int potentialDiambiguation = move.end + (directionValues[dir] * numSquares);

            // Do not check moves that are off of the board
            if (potentialDiambiguation < A8 || potentialDiambiguation > H1)
            {
                // Stop Searching in this direction
                break;
            }
            // Do not check moves that have wrapped around the board
            else if (potentialDiambiguation % Chess::NumFiles == wrapFile)
            {
                // Stop Searching in this direction
                break;
            }
            // Do not check the move from the potential move back to where this piece is
            else if (potentialDiambiguation == position)
            {
                continue;
            }
            // Check if a piece is encounters (Only the first piece matters for this search)
            else if (charBoard[potentialDiambiguation] != '-')
            {
                // Only check for other pieces of the same color
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
                break;
            }
        } // Squares in Direction
    } // Directions
}