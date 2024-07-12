//-------------------------------------------------------
//
// File: ChessTypes.h
// Author: Edward Koch
// Description: Holds the Definitions of chess types
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    03/04/23    Initial Creation of Chess Types
//-------------------------------------------------------
#pragma once

#include <vector>

// Holds the 2 Possible colors of pieces
enum Colors
{
    WHITE,
    BLACK
};

// IDs of each Piece
enum PieceTypes
{
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    NUM_PIECES
};

// Holds the notation of squares as indecies of the board
enum Squares
{
    A8,	B8,	C8,	D8,	E8,	F8,	G8,	H8,
    A7,	B7,	C7,	D7,	E7,	F7,	G7,	H7,
    A6,	B6,	C6,	D6,	E6,	F6,	G6,	H6,
    A5,	B5,	C5,	D5,	E5,	F5,	G5,	H5,
    A4,	B4,	C4,	D4,	E4,	F4,	G4,	H4,
    A3,	B3,	C3,	D3,	E3,	F3,	G3,	H3,
    A2,	B2,	C2,	D2,	E2,	F2,	G2,	H2,
    A1,	B1,	C1,	D1,	E1,	F1,	G1,	H1, INVALID

};
// Define a vector of squares, for many purposes
typedef std::vector<Squares> sqVector;

// The modes of a chess game
enum GameModes
{
    PVP = 1, // Player vs Player
    PVC = 2, // Player vs Computer
    CVC = 3  // Computer vs Computer
};

// The different end results of a game
enum GameResults
{
    IN_PROGRESS,
    WHITE_WIN,
    BLACK_WIN,
    DRAW_50MOVES,
    DRAW_REPETITION,
    DRAW_STALEMATE
};

namespace Chess
{
    // Determines the enemy color of a given color
    static const Colors enemy[2] = { BLACK, WHITE };

    // The size of a standard Chess Board
    const int NumRanks = 8;
    const int NumFiles = 8;
    const int NumSquares = NumRanks * NumFiles;

    // Holds a Mapping of File index to Letter
    static const char File[NumFiles] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };

    // Holds a mapping of Rank index to Number
    static const char Rank[NumRanks] = { '8', '7', '6', '5', '4', '3', '2', '1' };

    // Holds a maping of Squares to their String representation
    static const char* SquareChars[NumSquares] =
    {
        "A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8",
        "A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
        "A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
        "A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
        "A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
        "A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
        "A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
        "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1"
    };

    // The default board position for all chess games
    static const char* startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    // Maps Endgame conditions to their String representation
    static const char* EndgameChars[DRAW_STALEMATE + 1] =
    {
        "GAME IN PROGRESS",
        "WHITE WINS",
        "BLACK WINS",
        "DRAW BY 50 MOVE RULE",
        "DRAW BY 3-FOLD REPETITION",
        "DRAW BY STALEMATE"
    };
}
