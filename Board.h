//-------------------------------------------------------
//
// File: Board.h
// Author: Edward Koch
// Description: Holds the declaration of the Board Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    03/03/23    Initial Creation of Board class
//-------------------------------------------------------
#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "ChessTypes.h"
#include "Fen.h"
#include "Move.h"

// Forward Declarations
class Piece;

class Board
{
public:
    // @purpose: Construct and initalize all pieces on the board
    // @parameters: string fen that holds a fen position of a board
    // @return: None
    Board(const char* fen = Chess::startingFen);
   
    // Default Destructor
    ~Board();

    // @purpose: Construct and initalize all pieces on the board
    // @parameters: None
    // @return: None
    void initializeBoard();

    // @purpose: prints the current state of the board to the console
    // @parameters: None
    // @return: None
    void printBoardToConsole();

    // @purpose: prints the possible moves of all pieces
    // @parameters: Colors turn - which color is moving
    // @return: None
    void printAllMoves(Colors turn);

    // @purpose: Prints the move history of the game
    // @parameters: If there should be a newline after every move
    // @return: None
    void printMoveHistory(bool newline);

    // @purpose: Updates the FEN given the game state
    // @parameters: None
    // @return: None
    void calculateFen();

    // @purpose: Returns the FEN object of the game state
    // @parameters: None
    // @return: Fen* FEN representation
    Fen* getFen();

    // @purpose: Returns the status of the game
    // @parameters: None
    // @return: GameResults status
    GameResults getGameStatus();

    // @purpose: Generates the FEN.piecePlacement representation of the Board
    // @parameters: None
    // @return: const char* FEN representation
    const char* getPiecePlacement();

    // @purpose: Gets the Fen Representation of the board state
    // @parameters: None
    // @return: const char* char representation
    const char* getFenString();

    // @purpose: Gets the 64-length character array that represents the board state
    // @parameters: None
    // @return: const char* char representation
    const char* getCharBoard();

    // @purpose: Generates a 64-length character array that represents the board state
    // @parameters: None
    // @return: const char* char representation
    void setCharBoard();

    // @purpose: Tells the board that a king is in check from a piece
    // @parameters: Path from checker to king, color of king
    // @return: None
    void addCheckPath(sqVector checkPath, Colors king);

    // @purpose: Gets all of the check paths from an enemy to a king
    // @parameters: Color of king
    // @return: vector of paths from which the king is in check
    std::vector<sqVector>* getCheckPaths(Colors king);

    // @purpose: Get a reference to a piece on the board
    // @parameters: Square where the piece is
    // @return: Piece reference
    Piece* getPieceAt(Squares pos);

    // @purpose: Get a reference to the king of a given color
    // @parameters: Color of the King
    // @return: Piece reference
    Piece* getKing(Colors color);

    // @purpose: Calculates all moves of all pieces of one color
    // @parameters: Color of the pieces to calculate moves for
    // @return: None
    void calculateAllMoves(Colors pieceColor);

    // @purpose: Get all moves of all pieces of one color
    // @parameters: Color of the pieces to calculate moves for
    // @return: vector of all moves that the color can move to
    mvVector* getAllMoves(Colors pieceColor);

    // @purpose: Get the move hisotry for the game
    // @parameters: None
    // @return: Vector of moves that have been played
    mvVector* getMoveHistory();

    // @purpose: Get all squares that one piece is protecting
    // @parameters: Color of the pieces to calculate moves for
    // @return: vector of all squares that the color can move to
    sqVector* getAllProtected(Colors pieceColor);

    // @purpose: Moves a piece from one square to another
    // @parameters: Move object with all move information
    // @return: None
    void makeMove(Move move);

    // @purpose: Un-makes the last move
    // @parameters: None
    // @return: None
    void undoMove();

    // @purpose: Checks if the game is over
    // @parameters: None
    // @return: Enumeration of game ends
    GameResults checkEndgame();

private:
    // Allow a chess Game object to access the board directly
    friend class Game;

    // array to hold the game state
    Piece* board[Chess::NumRanks*Chess::NumFiles];
    /*
    * board is essentialy an 8x8 array that represents a chess board as depicted below
    *   - - - - - - - - 
    * 8|r n b q k b n r |
    * 7|p p p p p p p p |
    * 6|. . . . . . . . |
    * 5|. . . . . . . . |
    * 4|. . . . . . . . |
    * 3|. . . . . . . . |
    * 2|P P P P P P P P |
    * 1|R N B Q K B N R |
    *   - - - - - - - - 
    *   A B C D E F G H
    */

    // pointer to the Kings
    Piece* WhiteKing;
    Piece* BlackKing;

    // Holds a 64-length character representation of the current game state
    char* charBoard;

    // Holds the Game state of the board
    Fen fen;
    GameResults gameStatus;

    // Holds the move history for this board
    mvVector moveHistory;

    // Holds all the possible moves for a given color
    mvVector whiteMoves;
    mvVector blackMoves;

    // Holds all the squares that a given color is protecting
    sqVector whiteProtecting;
    sqVector blackProtecting;

    // Holds The paths from a checker to a king
    std::vector<sqVector> whiteCheckPaths;
    std::vector<sqVector> blackCheckPaths;

    // @purpose: Moves a piece from one square to another
    // @parameters: Square start position, Square end position
    // @return: None
    void movePiece(Squares start, Squares end);

    // @purpose: Handle all En Passant Actions
    // @parameters: Move object to update
    // @return: None
    void checkEnPassant(Move* move);

    // @purpose: Handle all Castling Actions
    // @parameters: Move object to update
    // @return: None
    void checkCastling(Move* move);

    // @purpose: Promotes a Pawn that reaches the end of the board
    // @parameters: Move object to update
    // @return: None
    void promotePawn(Move* move);

    // @purpose: Clears the check conditions and pinnedPath from all pieces
    // @parameters: None
    // @return: None
    void clearAllConditions();

    // @purpose: Processes all Endgame functionality
    // @parameters: None
    // @return: None
    void processEndgame();


};

#endif //BOARD_H