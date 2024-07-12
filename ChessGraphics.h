//-------------------------------------------------------
//
// File: Game.h
// Author: Edward Koch
// Description: Holds the declaration of the ChessGraphics Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    03/07/23    Initial Creation of ChessGraphics class
//-------------------------------------------------------
#ifndef CHESS_GRAPHICS_H
#define CHESS_GRAPHICS_H

#include <SFML/Graphics.hpp>

#include "ChessTypes.h"

// Forward Declarations
class Board;
class Piece;

class ChessGraphics
{
public:
    // Default Constructor
    ChessGraphics();

    // Default Destructor
    ~ChessGraphics();

    // @purpose: Gets a pointer to the window for event handling
    // @parameters: None
    // @return: Render Window pointer to window object
    sf::RenderWindow* getWindow() { return window; }

    // @purpose: Coverts Mouse coordinates to a Chess Square
    // @parameters: Mouse Event
    // @return: Chess Square
    Squares mouseToSquare(sf::Event event);

    // @purpose: Coverts Mouse coordinates to a Chess Square
    // @parameters: Mouse Event
    // @return: Chess Square
    sf::Vector2f squareToMouse(Squares sq);

    // @purpose: Draws the Chess Board and Pieces
    // @parameters: Board,  current piece
    // @return: None
    void drawGame(Board* board, Piece* currentPiece);

    // @purpose: Draws the Chess Board and Pieces
    // @parameters: Game Mode, Color Selection
    // @return: None
    void drawMainMenu();

    // @purpose: Draws the Chess Board and Pieces
    // @parameters: Game Mode, Color Selection
    // @return: None
    void drawPvcMenu(Colors human, int depth);

    // @purpose: Draws the Chess Board and Pieces
    // @parameters: Game Mode, Color Selection
    // @return: None
    void drawCvcMenu(int whiteDepth, int blackDepth);

    // @purpose: Display options for the user to promote a Pawn to
    // @parameters: Color of Piece to pick
    // @return: Character representation of the piece to promote to
    char getPawnPromotionOption(Colors color);

private:

    // Holds a window object to draw on
    sf::RenderWindow* window;
    sf::Font font;
    int titleFontSize;
    int wordFontSize;

    // Holds the board squares to draw
    sf::RectangleShape squares[Chess::NumSquares];
    sf::RectangleShape highlightSquares[Chess::NumSquares];
    sf::Text rankMarks[Chess::NumRanks];
    sf::Text fileMarks[Chess::NumFiles];

    // Holds all of the Piece images
    sf::Texture whiteKing;
    sf::Texture blackKing;
    sf::Texture whiteQueen;
    sf::Texture blackQueen;
    sf::Texture whiteRook;
    sf::Texture blackRook;
    sf::Texture whiteBishop;
    sf::Texture blackBishop;
    sf::Texture whiteKnight;
    sf::Texture blackKnight;
    sf::Texture whitePawn;
    sf::Texture blackPawn;

    sf::Sprite* pieceSprites[Chess::NumSquares];

    // Holds the size of the window and squares
    const int windowSize;
    const float squareSize;

    // @purpose: Prepares base board for drawing
    // @parameters: None
    // @return: None
    void prepareBoard();

    // @purpose: Prepares the piece images
    // @parameters: None
    // @return: None
    void preparePieces();

    // @purpose: Draws the squares and letters
    // @parameters: None
    // @return: None
    void drawBoard();

    // @purpose: Draws the rank and file markings on the board
    // @parameters: None
    // @return: None
    void drawMarkings();

    // @purpose: Draws the Peices based on the board
    // @parameters: Character representation of the board
    // @return: None
    void drawPieces(const char* board);

    // @purpose: Draws highlighted squares for potential moves
    // @parameters: Board Object, Current piece to draw around
    // @return: None
    void drawHighlights(Board* board, Piece* currentPiece);
};

#endif //CHESS_GRAPHICS_H