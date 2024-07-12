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
#include <cctype>

#include "Pawn.h"

// Default Constructor
Pawn::Pawn(Colors colorIn, Squares posIn)
    : Piece('p', colorIn, posIn)
{
    type = PAWN;
}

// Default Destructor
Pawn::~Pawn()
{

}

// @purpose: Calculate all possible moves given a character representation of the boad
// @parameters: Character Representation of the board, Color of piece that is moving
// @return: None
void Pawn::calculateMoves(Board* board, Colors turn)
{
    // Clear all possible moves
    moves.clear();
    protecting.clear();

    // Get Character Representation of the board
    const char* charBoard = board->getCharBoard();

    // Path of the attacking piece to the King - 
    // For a Pawn, only the current position is in the path, as Pawn captures cant be blocked
    sqVector pathToKing;
    pathToKing.push_back(position);

    // Use a modifier to commonize code between colors
    int modifier = 0;
    bool isStartingRank = false;
    if (color == WHITE)
    {
        // White pieces move up the board, negtively
        modifier = -1;
        if (position >= A2 && position <= H2)
        {
            isStartingRank = true;
        }
    }
    else
    {
        // Black pieces move up the board, positively
        modifier = 1;
        if (position >= A7 && position <= H7)
        {
            isStartingRank = true;
        }
    }

    // Calculate the positions of all possible moves
    int forwardOne = position + (8 * modifier);
    int forwardTwo = position + (16 * modifier);
    int captureRight = position + (7 * modifier);
    int captureLeft = position + (9 * modifier);

    // Ignore moves that are off of the board
    if (forwardOne >= A8 && forwardOne <= H1)
    {
        // On a normal turn, a Pawn may move forward one square if unimpeaded - and is otherwise legal
        if (charBoard[forwardOne] == '-' &&
            isLegalMove(board, (Squares)forwardOne))
        {
            // Add the potential move to the move vector
            addPawnMoves(forwardOne);
        }

        // On first move, a Pawn may move 2 squares foward  if unimpeaded - and is otherwise legal
        if (isStartingRank && charBoard[forwardOne] == '-' && charBoard[forwardTwo] == '-' && isLegalMove(board, (Squares)forwardTwo))
        {
            // Add the potential move to the move vector
            addPawnMoves(forwardTwo);
        }
    }

    // Ignore positions that are off of the board or wrapped around - or is otherwise illegal
    if (captureRight >= A8 && captureRight <= H1  &&
        abs(position % Chess::NumFiles - captureRight % Chess::NumFiles) <= 1 &&
        isLegalMove(board, (Squares)captureRight))
    {
        // On a normal turn, a Pawn may capture forward one diagonally one
        if (charBoard[captureRight] != '-' && isupper(charBoard[captureRight]) != isupper(charId))
        {
            // Add the potential move to the move vector
            addPawnMoves(captureRight);

            // If the piece is a King, put it in check
            if (board->getPieceAt((Squares)captureRight)->getType() == KING)
            {
                pathToKing.push_back((Squares)captureRight);
                board->addCheckPath(pathToKing, Chess::enemy[color]);
            }
        }
        // Pawns may capture en Passant
        else if (captureRight == board->getFen()->enPassantTarget)
        {
            // En Passant captures can potentially cause check to occur
            // as current move generation for the enemy sliding piece would
            // not catch a K-Pp-r pin, if P took p en Passant
            // Check if this Pawn's King is to the left of this pawn
            if (safeEnPassant(captureRight, board))
            {
                // Add the potential move to the move vector
                addPawnMoves(captureRight);
                moves.back().isEnPassant = true;
                protecting.push_back((Squares)captureRight);
            }
        }
        // If the square will be protected by the pawn
        else
        {
            protecting.push_back((Squares)captureRight);
        } 
    }
    // Ignore positions that are off of the board or wrapped around - or is otherwise illegal
    if (captureLeft >= A8 && captureLeft <= H1 &&
        abs(position % Chess::NumFiles - captureLeft % Chess::NumFiles) <= 1 &&
        isLegalMove(board, (Squares)captureLeft))
    {
        // On a normal turn, a Pawn may capture forward one diagonally one
        if (charBoard[captureLeft] != '-' && isupper(charBoard[captureLeft]) != isupper(charId))
        {
            // Add the potential move to the move vector
            addPawnMoves(captureLeft);

            // If the piece is a King, put it in check
            if (board->getPieceAt((Squares)captureLeft)->getType() == KING)
            {
                pathToKing.push_back((Squares)captureLeft);
                board->addCheckPath(pathToKing, Chess::enemy[color]);
            }
        }
        // Pawns may capture en Passant
        else if (captureLeft == board->getFen()->enPassantTarget)
        {
            // En Passant captures can potentially cause check to occur
            // as current move generation for the enemy sliding piece would
            // not catch a r-pP-k pin, if P took p en Passant
            if (safeEnPassant(captureLeft, board))
            {
                // Add the potential move to the move vector
                addPawnMoves(captureLeft);
                moves.back().isEnPassant = true;
                protecting.push_back((Squares)captureLeft);
            }
        }
        // If the square will be protected by the pawn
        else
        {
            protecting.push_back((Squares)captureLeft);
        }
    }
}

// @purpose: Check if En Passant will cause a check
// @parameters: board index to move to, board object to search
// @return: None
bool Pawn::safeEnPassant(int end, Board* board)
{
    // En Passant captures can potentially cause check to occur
    // as current move generation for the enemy sliding piece would
    // not catch a K-Pp-r pin, if P took p en Passant

    // If this King is not on the same rank, than it is a safe move
    Piece* king = board->getKing(color);
    if (position / Chess::NumRanks != king->getPos() / Chess::NumRanks)
    {
        return true;
    }

    // If the King is to the left of the pawn 
    if (king->getPos() < position)
    {
        // Check if there is an enemy QUEEN or ROOK that would be attacking the King 
        // from the right if the pawn took e.p. (Go right until wrapping around to 0)
        for (int i = position + 1; i % Chess::NumFiles > 0; ++i)
        {
            Piece* piece = board->getPieceAt((Squares)i);

            if (piece == 0) continue;

            // There is another friendly piece blocking
            if (piece->getColor() == color)
            {
                return true;
            }
            // If this piece is the En Passant pawn - ignore it
            else if (piece->getType() == PAWN && i == position + 1 && 
                     abs(board->getFen()->enPassantTarget - i) == Chess::NumFiles)
            {
                continue;
            }
            // if there is a Q/R that could attack the King, continue processing
            else if (piece->getType() == QUEEN || piece->getType() == ROOK)
            {
                // Check if there are any pieces to the left of the pawn that would block check
                for (int j = position - 1; j > king->getPos(); --j)
                {
                    Piece* piece = board->getPieceAt((Squares)j);
                    if (piece == 0) continue;

                    // Skip the  En Passant Pawn
                    if (piece->getType() == PAWN && 
                        piece->getColor() != color && 
                        j == position - 1 &&
                        abs(board->getFen()->enPassantTarget - j) == Chess::NumFiles)
                    {
                        continue;
                    }
                    // If there is any piece blocking, then it is safe to en passant
                    else
                    {
                        return true;
                    }
                }
                return false;
            }
            // If there is a non-Q/R enemy piece that could not attack the king after e.p.
            else
            {
                return true;
            }
        }
    }
    // If the King is to the Right of the pawn
    if (king->getPos() > position)
    {
        // Check if there is an enemy QUEEN or ROOK that would be attacking the King 
        // from the left if the pawn took e.p. (Go left until wrapping around to 7)
        for (int i = position - 1; i % Chess::NumFiles < 7; --i)
        {
            Piece* piece = board->getPieceAt((Squares)i);

            if (piece == 0) continue;

            // There is another friendly piece blocking
            if (piece->getColor() == color)
            {
                return true;
            }
            // If this piece is the En Passant pawn - ignore it
            else if (piece->getType() == PAWN && i == position - 1 &&
                     abs(board->getFen()->enPassantTarget - i) == Chess::NumFiles)
            {
                continue;
            }
            // if there is a Q/R that could attack the King, continue processing
            else if (piece->getType() == QUEEN || piece->getType() == ROOK)
            {
                // Check if there are any pieces to the right of the pawn that would block check
                for (int j = position + 1; j < king->getPos(); ++j)
                {
                    Piece* piece = board->getPieceAt((Squares)j);
                    if (piece == 0) continue;

                    // Skip the  En Passant Pawn
                    if (piece->getType() == PAWN && 
                        piece->getColor() != color && 
                        j == position + 1 &&
                        abs(board->getFen()->enPassantTarget - j) == Chess::NumFiles)
                    {
                        continue;
                    }
                    // If there is any piece blocking, then it is safe to en passant
                    else
                    {
                        return true;
                    }
                }
                // If there is no piece to block, it is not safe
                return false;
            }
            // If there is a non-Q/R enemy piece that could not attack the king after e.p.
            else
            {
                return true;
            }
        }
    }
    return true;
}


// @purpose: Add All possible moves to the move list - including opssible promotion
// @parameters: Square to move to
// @return: None
void Pawn::addPawnMoves(int end)
{
    // Handle Pawn promotion
    if ((color == WHITE &&
         end >= A8 && end <= H8) ||
        (color == BLACK &&
         end >= A1 && end <= H1))
    {
        // Add All Possible Promotions
        const int numOptions = 4;
        char promotionTargets[numOptions] = { 'Q', 'R', 'B', 'N' };

        for (int i = 0; i < numOptions; ++i)
        {
            char option = promotionTargets[i];
            if (color == BLACK)
            {
                option = tolower(option);
            }

            Move move;
            move.pieceMoved = this;
            move.start = position;
            move.end = (Squares)end;
            move.targetPromotion = option;

            moves.push_back(move);
        }
    }
    else
    {
        // Add move as normal
        addMove(end);
    }

}

