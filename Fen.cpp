//-------------------------------------------------------
//
// File: Fen.h
// Author: Edward Koch
// Description: Holds the definition of the Fen Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    04/04/23    Initial Creation of Fen class
//-------------------------------------------------------
#include "Fen.h"
#include <cctype>

// Default Constructor
// @parameters: const char* Fen String
Fen::Fen(const char* fenString)
    : fen{ 0 },
      piecePlacement { 0 },
      activeColor(WHITE),
      kingCastleWhite(false),
      queenCastleWhite(false),
      kingCastleBlack(false),
      queenCastleBlack(false),
      enPassantTarget(INVALID),
      halfmoveClock(0),
      fullmoveNumber(0)
{
    parseFen(fenString);
}

// Default Constructor
Fen::Fen()
    : fen{ 0 },
    piecePlacement{ 0 },
    activeColor(WHITE),
    kingCastleWhite(false),
    queenCastleWhite(false),
    kingCastleBlack(false),
    queenCastleBlack(false),
    enPassantTarget(INVALID),
    halfmoveClock(0),
    fullmoveNumber(0)
{

}

// Default Destructor
Fen::~Fen()
{

}

// @purpose: Reads in a FEN and populates the member variables
// @parameters: const char* Fen String
// @return: None
void Fen::parseFen(const char* fenString)
{
    // Initalize the index to -1 inorder to use pre-increment
    int index = -1;

    // Track which section of the Fen we are on
    int section = 0;

    // Loop through the entire fen string
    while (fenString[++index] != '\0')
    {
        fen[index] = fenString[index];

        // Skip all spaces
        if (fenString[index] == ' ')
        {
            index++;
            section++;
        }

        switch (section)
        {
        case 0: // Piece Placement
            piecePlacement[index] = fenString[index];
            break;

        case 1: // Active Color
            if (fenString[index] == 'w')
            {
                activeColor = WHITE;
            }
            else
            {
                activeColor = BLACK;
            }
            break;

        case 2: // Castleing Rights
            if (fenString[index] == 'K')
            {
                kingCastleWhite = true;
            }
            else if (fenString[index] == 'Q')
            {
                queenCastleWhite = true;
            }
            else if (fenString[index] == 'k')
            {
                kingCastleBlack = true;
            }
            else if (fenString[index] == 'q')
            {
                queenCastleBlack = true;
            }
            else if (fenString[index] == '-')
            {
                kingCastleWhite = false;
                queenCastleWhite = false;
                kingCastleBlack = false;
                queenCastleBlack = false;
            }
            break;

        case 3: // enPassant Target
            if (fenString[index] == '-')
            {
                enPassantTarget = INVALID;
            }
            else
            {
                parseEnPassant(fenString[index], fenString[index+1]);
                ++index;
            }
            break;

        case 4: // Halfmove Clock
            halfmoveClock = 0;
            while (fenString[index] != ' ' &&
                   fenString[index] != '\0')
            {
                halfmoveClock = (fenString[index++] - '0') + (halfmoveClock * 10);
            }
            section++;
            break;

        case 5: // Fullmove Number
            fullmoveNumber = 0;
            while (fenString[index] != ' ' &&
                   fenString[index] != '\0')
            {
                fullmoveNumber = (fenString[index++] - '0') + (fullmoveNumber * 10);
            }
            section++;
            break;
        } 
    }
}

// @purpose: Constructs a FEN based on the current state of the variables
// @parameters: const char* fen representation of the pieces
// @return: const char* Fen String
const char* Fen::constructFen(const char* pieces)
{
    int fenIndex = -1;
    // Piece Placement
    for (int i = 0; i < 71; ++i)
    {
        if (pieces[i] == '\0' || pieces[i] < 0)
        {
            break;
        }
        fen[++fenIndex] = pieces[i];
        piecePlacement[fenIndex] = pieces[i];
    }
    fen[++fenIndex] = ' ';

    // Active Color
    if (activeColor == WHITE)
    {
        fen[++fenIndex] = 'w';
    }
    else
    {
        fen[++fenIndex] = 'b';
    }
    fen[++fenIndex] = ' ';

    // Castleing Rights
    // Count number of Castles
    int preCastleFenIndex = fenIndex;
    if (kingCastleWhite)
    {
        fen[++fenIndex] = 'K';
    }
    if (queenCastleWhite)
    {
        fen[++fenIndex] = 'Q';
    }
    if (kingCastleBlack)
    {
        fen[++fenIndex] = 'k';
    }
    if (queenCastleBlack)
    {
        fen[++fenIndex] = 'q';
    }
    // Put a dash for empty castling rights
    if (fenIndex == preCastleFenIndex)
    {
        fen[++fenIndex] = '-';
    }
    fen[++fenIndex] = ' ';

    // En Passant Target
    if (enPassantTarget == INVALID)
    {
        fen[++fenIndex] = '-';
    }
    else
    {
        fen[++fenIndex] = tolower(Chess::SquareChars[enPassantTarget][0]);
        fen[++fenIndex] = tolower(Chess::SquareChars[enPassantTarget][1]);
    }
    fen[++fenIndex] = ' ';

    // Half Move Clock
    if (halfmoveClock == 0)
    {
        fen[++fenIndex] = '0';
    }
    else
    {
        int tmpNum = halfmoveClock;
        if (tmpNum > 1000)
        {
            fen[++fenIndex] = (tmpNum / 1000) + 48;
            tmpNum = tmpNum % 1000;
        }
        if (tmpNum > 100)
        {
            fen[++fenIndex] = (tmpNum / 100) + 48;
            tmpNum = tmpNum % 100;
        }
        if (tmpNum > 10)
        {
            fen[++fenIndex] = (tmpNum / 10) + 48;
            tmpNum = tmpNum % 10;
        }
        if (tmpNum > 0)
        {
            fen[++fenIndex] = (tmpNum / 1) + 48;
            tmpNum = tmpNum % 1;
        }
        if (tmpNum == 0 && halfmoveClock == 0)
        {
            fen[++fenIndex] = (tmpNum)+48;
        }
    }
    fen[++fenIndex] = ' ';

    // Full Move Number
    if (fullmoveNumber == 0)
    {
        fen[++fenIndex] = '0';
    }
    else
    {
        int tmpNum = fullmoveNumber;
        if (tmpNum > 1000)
        {
            fen[++fenIndex] = (tmpNum / 1000) + 48;
            tmpNum = tmpNum % 1000;
        }
        if (tmpNum > 100)
        {
            fen[++fenIndex] = (tmpNum / 100) + 48;
            tmpNum = tmpNum % 100;
        }
        if (tmpNum > 10)
        {
            fen[++fenIndex] = (tmpNum / 10) + 48;
            tmpNum = tmpNum % 10;
        }
        if (tmpNum > 0)
        {
            fen[++fenIndex] = (tmpNum / 1) + 48;
            tmpNum = tmpNum % 1;
        }
        if (tmpNum == 0)
        {
            fen[++fenIndex] = (tmpNum) + 48;
        }
    }
    
    // Fill the rest of the string with Null Characters
    while (++fenIndex < MAX_FEN_LENGTH)
    {
        fen[fenIndex] = '\0';
    }

    return 0;
}

// @purpose: Parses a Fen enpassant square to a Squares type
// @parameters: file and rank as characters
// @return: None
void Fen::parseEnPassant(char file, char rank)
{
    char sq[2] = { 0 };
    sq[0] = toupper(file);
    sq[1] = toupper(rank);

    const char* sqChar = &sq[0];

    for (int square = A8; square <= H1; ++square)
    {
        if (Chess::SquareChars[square][0] == sqChar[0] &&
            Chess::SquareChars[square][1] == sqChar[1])
        {
            enPassantTarget = (Squares)square;
            return;
        }
    }
}
