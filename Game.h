//-------------------------------------------------------
//
// File: Game.h
// Author: Edward Koch
// Description: Holds the declaration of the Game Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    03/05/23    Initial Creation of Game class
//-------------------------------------------------------
#ifndef GAME_H
#define GAME_H

#include "ChessAI.h"
#include "ChessGraphics.h"
#include "ChessTypes.h"

// Forward Declarations
class Board;
class Piece;

class Game
{
public:
    // Default Constructor
    Game();

    // Default Destructor
    ~Game();

    // @purpose: Sets the fen of the board to play on
    // @parameters: fen string
    // @return: None
    void setFen(const char* fen);

    // @purpose: Draws Chess Board and handles turns/move making
    // @parameters: None
    // @return: None
    void play();

private:
    // Holds a board to play
    Board* board;

    // The mode to launch the game in
    GameModes mode;

    // The color of the Human Player
    Colors human;

    // ChessAI to play against
    ChessAI whiteAI;
    ChessAI blackAI;

    // AI Settings
    bool whiteAIEnabled;
    int whiteAIDepth;

    bool blackAIEnabled;
    int blackAIDepth;

    // Nescessary 'global' variables for passing between functions
    GameResults result;
    mvVector* currentMoves;
    Piece* currentPiece;

    // Holds a Graphics Driver to display all of the chess game
    ChessGraphics* drawer;

    // @purpose: Asks the User to select game settings
    // @parameters: None
    // @return: True if valid options, false otherwise
    bool getSettings();

    // @purpose: Asks the User to select PVP settings
    // @parameters: None
    // @return: True if start is pressed, false if right click
    bool getPvpSettings();

    // @purpose: Asks the User to select PVP settings
    // @parameters: None
    // @return: True if start is pressed, false if right click
    bool getPvcSettings();

    // @purpose: Asks the User to select PVP settings
    // @parameters: None
    // @return: True if start is pressed, false if right click
    bool getCvcSettings();

    // @purpose: Processes a Mousclick Input
    // @parameters: Event to process
    // @return: None
    void processMouseClick(sf::Event event);

    // @purpose: Processes a Left Click Input
    // @parameters: Event to process
    // @return: None
    void processLeftClick(sf::Event);

    // @purpose: Gets a piece from the baord at a given square
    // @parameters: Square to get the piece
    // @return: None
    void selectPieceAt(Squares sq);

    // @purpose: Processes a Right Click Input
    // @parameters: Event to process
    // @return: None
    void processRightClick(sf::Event);

    // @purpose: Moves a piece to a square
    // @parameters: Piece pointer, Square to mvoe to
    // @return: None
    void makeMove(Piece* piece, Squares square);
};

#endif //GAME_H