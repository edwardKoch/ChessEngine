//-------------------------------------------------------
//
// File: Move.h
// Author: Edward Koch
// Description: Holds the definition of the Move Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    04/08/23    Initial Creation of Move class
//-------------------------------------------------------
#include <cctype>

#include "Logger.h"
#include "Move.h"
#include "Piece.h"

// Default Constructor
Move::Move()
    : pieceMoved(0),
      start(INVALID),
      end(INVALID),
      rankDisambig(0),
      fileDisambig(0),
      pieceCaptured(0),
      isEnPassant(false),
      pieceCastled(0),
      castledFrom(INVALID),
      piecePromoted(0),
      targetPromotion('\0'),
      oldFen()
{

}

    // Default Destructor
Move::~Move()
{

}

// @purpose: Prints the move in algebraic notation
// @parameters: Boolean whether or not to print a newline after
// @return: None
void Move::printMove(bool newline)
{
    // Print the move number if white
    if (pieceMoved->getColor() == WHITE)
    {
        Logger::log("%i. ", oldFen.fullmoveNumber);
    }
    // Handle Castles specially
    if (pieceCastled != 0)
    {
        // Print Kingside Castle
        if (start < end)
        {
            Logger::log("O-O");
        }
        // Print Queenside Castle
        else
        {
            Logger::log("O-O-O");
        }

        // Print a newline or space
        if (newline)
        {
            Logger::log("\n");
        }
        else
        {
            Logger::log(" ");
        }
        return;
    }
    
    // For non-pawns, print the character id of the piece
    if (pieceMoved->getType() != PAWN)
    {
        Logger::log("%c", toupper(pieceMoved->getCharId()));
    }
    else
    {
        // for Pawn captures, print the file identifier first
        if (pieceCaptured != 0)
        {
            Logger::log("%c", tolower(Chess::File[start % Chess::NumFiles]));
        }
    }

    // Add the File disambiguation
    if (fileDisambig != 0)
    {
        Logger::log("%c", tolower(fileDisambig));
    }

    // Add the Rank disambiguation
    if (rankDisambig != 0)
    {
        Logger::log("%c", tolower(rankDisambig));
    }

    // If there was a capture, print an 'x'
    if (pieceCaptured != 0)
    {
        Logger::log("x");
    }

    // Print the square that was moved to
    Logger::log("%c%c", tolower(Chess::File[end % Chess::NumFiles]), 
                        Chess::Rank[end / Chess::NumRanks]);

    // If the move was En-Passant, specify with 'e.p.'
    if (isEnPassant)
    {
        Logger::log("e.p.");
    }

    // If there was a pawn promotion, specify the piece chose
    if (piecePromoted != 0)
    {
        Logger::log("=%c", toupper(piecePromoted->getCharId()));
    }

    // Print a newline or space
    if (newline)
    {
        Logger::log("\n");
    }
    else
    {
        Logger::log(" ");
    }
}

// @purpose: Prints the move in algebraic notation
// @parameters: None
// @return: None
void Move::printDebug(bool newline)
{
    Logger::log("PieceMoved:\n");
    pieceMoved->printPiece();
    Logger::log("Start: %s\n", Chess::SquareChars[start]);
    Logger::log("End: %s\n", Chess::SquareChars[end]);
    Logger::log("PieceCaptured: %i\n", pieceCaptured);
    Logger::log("isEnPassant: %i\n", isEnPassant);
    Logger::log("pieceCastled: %i\n", pieceCastled);
    Logger::log("castledFrom: %i\n", castledFrom);
    Logger::log("piecePromoted: %i\n", piecePromoted);
    Logger::log("targetPromotion: %c\n", targetPromotion);

    // Print a newline or space
    if (newline)
    {
        Logger::log("\n");
    }
    else
    {
        Logger::log(" ");
    }
}

// Comparator function
bool Move::operator == (const Move& m) 
{
    return (m.pieceMoved == pieceMoved &&
            m.start == start &&
            m.end == end &&
            m.targetPromotion == targetPromotion);
}
