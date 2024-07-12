//-------------------------------------------------------
//
// File: Fen.h
// Author: Edward Koch
// Description: Holds the declaration of the Fen Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    04/04/23    Initial Creation of Fen class
//-------------------------------------------------------
#ifndef FEN_H
#define FEN_H

#include "ChessTypes.h"

class Fen
{
public:
    // Default Constructor
    // @parameters: const char* Fen String
    Fen(const char* fenString);

    // Default Constructor
    Fen();

    // Default Destructor
    ~Fen();

    // @purpose: Reads in a FEN and populates the member variables
    // @parameters: const char* Fen String
    // @return: None
    void parseFen(const char* fenString);

    // @purpose: Constructs a FEN based on the current state of the variables
    // @parameters: const char* fen representation of the pieces
    // @return: const char* Fen String
    const char* constructFen(const char* pieces);

    // Variables
    static const int MAX_FEN_LENGTH = 90;

    char fen[MAX_FEN_LENGTH];

    char piecePlacement[71];
    Colors activeColor;
    bool kingCastleWhite;
    bool queenCastleWhite;
    bool kingCastleBlack;
    bool queenCastleBlack;
    Squares enPassantTarget;
    int halfmoveClock;
    int fullmoveNumber;

private:

    // @purpose: Parses a Fen enpassant square to a Squares type
    // @parameters: file and rank as characters
    // @return: None
    void parseEnPassant(char file, char rank);
};

#endif // FEN_H

