//-------------------------------------------------------
//
// File: Game.h
// Author: Edward Koch
// Description: Holds the definition of the Game Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    03/07/23    Initial Creation of Game class
//-------------------------------------------------------
#include <iostream>

#include "Board.h"
#include "ChessGraphics.h"
#include "Game.h"
#include "Logger.h"
#include "Move.h"
#include "Piece.h"

// Default Constructor
Game::Game()
    : board(new Board(Chess::startingFen)),
      mode(CVC),
      human(WHITE), 
      whiteAI(WHITE, 5),
      blackAI(BLACK, 5),
      whiteAIEnabled(false),
      blackAIEnabled(false),
      whiteAIDepth(1),
      blackAIDepth(1),
      result(IN_PROGRESS),
      currentMoves(),
      currentPiece(0),
      drawer(new ChessGraphics())
{

}

// Default Destructor
Game::~Game()
{

}

// @purpose: Sets the fen of the board to play on
// @parameters: fen string
// @return: None
void Game::setFen(const char* fen)
{
    delete board;
    board = new Board(fen);
}

// @purpose: Draws Chess Board and handles turns/move making
// @parameters: None
// @return: None
void Game::play()
{
    sf::RenderWindow* window = drawer->getWindow();
    drawer->drawGame(board, currentPiece);

    if (!getSettings())
    {
        return;
    }

    // Draw the game initialy
    drawer->drawGame(board, currentPiece);

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }
            // Get Human input for moves
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                processMouseClick(event);
                // Update the GUI after interaction
                drawer->drawGame(board, currentPiece);
            }
            // If not Mouse Pressed or window exited
            else
            {
                // Exit the Event Loop
                break;
            }
        }

        // Handle White AI Turn - DISABLED
        if (board->fen.activeColor == WHITE && whiteAIEnabled && result == IN_PROGRESS)
        {
            // Display a 'Thinking' symbol so we know the AI is thinking
            Logger::log("White AI is Thinking...\n");

            // Get the Best AI move 
            board->makeMove(whiteAI.getBestMove(board, WHITE));
            result = board->getGameStatus();
            if (result != IN_PROGRESS)
            {
                Logger::log("%s!\n", Chess::EndgameChars[result]);
                board->printMoveHistory(true);
                board->printMoveHistory(false);
                Logger::log("%s\n", board->getFenString());
            }
        }
        // Handle Black AI Turn
        else if (board->fen.activeColor == BLACK && blackAIEnabled && result == IN_PROGRESS)
        {
            // Display a 'Thinking' symbol so we know the AI is thinking
            Logger::log("Black AI is Thinking...\n");

            // Get the Best AI move
            board->makeMove(blackAI.getBestMove(board, BLACK));
            result = board->getGameStatus();
            if (result != IN_PROGRESS)
            {
                Logger::log("%s!\n", Chess::EndgameChars[result]);
                board->printMoveHistory(true); 
                board->printMoveHistory(false);
                Logger::log("%s\n", board->getFenString());
            }
        }

        // Update the GUI after each frame
        drawer->drawGame(board, currentPiece);
    }
}

// @purpose: Asks the User to select game settings
// @parameters: None
// @return: True if valid options, false otherwise
bool Game::getSettings()
{
    enum Menus
    {
        MODE_SELECT,
        COLOR_SELECT
    };
    Menus menuMode = MODE_SELECT;

    sf::RenderWindow* window = drawer->getWindow();

    while (window->isOpen())
    {
        drawer->drawMainMenu();

        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }
            // Get Human input for moves
            else if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                // Get the Square that was clicked on
                Squares sq = drawer->mouseToSquare(event);
                switch (sq / Chess::NumRanks)
                {
                // Human vs Human drawn in the 5th File
                case 3:
                    mode = PVP;
                    if (getPvpSettings())
                    {
                        return true;
                    }
                    break;

                    // Human vs Human drawn in the 4th File
                case 4:
                    mode = PVC;
                    if (getPvcSettings())
                    {
                        return true;
                    }
                    break;

                    // Human vs Human drawn in the 3rd File
                case 5:
                    mode = CVC;
                    if (getCvcSettings())
                    {
                        return true;
                    }
                    break;

                default:
                    break;
                }
            }
        }
    }
    return true;
}

// @purpose: Asks the User to select PVP settings
// @parameters: None
// @return: True if start is pressed, false if right click
bool Game::getPvpSettings()
{
    whiteAIEnabled = false;
    blackAIEnabled = false;

    return true;
}

// @purpose: Asks the User to select PVP settings
// @parameters: None
// @return: True if start is pressed, false if right click
bool Game::getPvcSettings()
{
    int aiStrength = 1;
    int aiModifier = 1;
    whiteAIEnabled = false;
    blackAIEnabled = true;

    blackAIDepth = aiStrength + aiModifier;

    sf::RenderWindow* window = drawer->getWindow();
    while (window->isOpen())
    {
        drawer->drawPvcMenu(human, aiStrength);

        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }
            else if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Right)
            {
                // Go back to main menu
                return false;
            }
            // Get Left Click Input
            else if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                // Get the Square that was clicked on
                Squares sq = drawer->mouseToSquare(event);

                // Get the Color for the Human
                if (sq / Chess::NumRanks == 3)
                {
                    if (sq % Chess::NumFiles == 3)
                    {
                        human = WHITE;
                    }
                    else if (sq % Chess::NumFiles == 4)
                    {
                        human = BLACK;
                    }
                }
                // Get the AI Strength
                else if (sq / Chess::NumRanks == 5)
                {
                    int file = sq % Chess::NumFiles;
                    if (file >= 1 && file <= 6)
                    {
                        aiStrength = file;
                    }

                }
                // Start the Game
                else if (sq / Chess::NumRanks == 6 &&
                         (sq % Chess::NumFiles == 3 ||
                          sq % Chess::NumFiles == 4))
                {
                    // If the color is black, flip the board

                    if (human == WHITE)
                    {
                        whiteAIEnabled = false;
                        blackAIEnabled = true;

                        blackAIDepth = aiStrength;
                        blackAI.setDepth(blackAIDepth + aiModifier);
                    }
                    else
                    {
                        whiteAIEnabled = true;
                        blackAIEnabled = false;

                        whiteAIDepth = aiStrength;
                        whiteAI.setDepth(whiteAIDepth + aiModifier);
                    }

                    return true;
                }
            }
        }
    }

    return false;
}

// @purpose: Asks the User to select PVP settings
// @parameters: None
// @return: True if start is pressed, false if right click
bool Game::getCvcSettings()
{
    int aiModifier = 1;
    whiteAIEnabled = true;
    blackAIEnabled = true;

    sf::RenderWindow* window = drawer->getWindow();
    while (window->isOpen())
    {
        drawer->drawCvcMenu(whiteAIDepth, blackAIDepth);

        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }
            else if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Right)
            {
                // Go back to main menu
                return false;
            }
            else if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                // Get the Square that was clicked on
                Squares sq = drawer->mouseToSquare(event);

                // Get the White AI Strenght
                if (sq / Chess::NumRanks == 2)
                {
                    int file = sq % Chess::NumFiles;
                    if (file >= 1 && file <= 6)
                    {
                        whiteAIDepth = file;
                    }
                }
                // Get the Black AI Strength
                else if (sq / Chess::NumRanks == 4)
                {
                    int file = sq % Chess::NumFiles;
                    if (file >= 1 && file <= 6)
                    {
                        blackAIDepth = file;
                    }

                }
                // Start the Game
                else if (sq / Chess::NumRanks == 6 &&
                    (sq % Chess::NumFiles == 3 ||
                        sq % Chess::NumFiles == 4))
                {
                    whiteAI.setDepth(whiteAIDepth + aiModifier);
                    blackAI.setDepth(blackAIDepth + aiModifier);
                    return true;
                }

            }
        }
    }

    return false;
}

// @purpose: Processes a Mousclick Input
// @parameters: Event to process
// @return: None
void Game::processMouseClick(sf::Event event)
{
    // Handle Left Clicks to select pieces and moves
    if (event.mouseButton.button == sf::Mouse::Left)
    {
        processLeftClick(event);
    }
    else if (event.mouseButton.button == sf::Mouse::Right)
    {
        processRightClick(event);
    }

    return;
}

// @purpose: Processes a Left Click Input
// @parameters: Event to process
// @return: None
void Game::processLeftClick(sf::Event event)
{
    // Get the Square that was clicked on
    Squares sq = drawer->mouseToSquare(event);

    // If there is a current piece selected
    if (currentPiece != 0)
    {
        // Check if the mouse click is in the set of moves
        if (currentPiece->isSquareInMoves(sq))
        {
            // Make the move
            makeMove(currentPiece, sq);
            result = board->getGameStatus();
            if (result != IN_PROGRESS)
            {
                Logger::log("%s!\n", Chess::EndgameChars[result]);
                board->printMoveHistory(true);
                board->printMoveHistory(false);
                Logger::log("%s\n", board->getFenString());
            }
            currentPiece = 0;
            currentMoves = 0;
        }
        else
        {
            // Select the piece on that square
            selectPieceAt(sq);
        }
    }
    // If there is no piece selected currently
    else
    {
        // Select the piece at that square
        selectPieceAt(sq);
    }
}

// @purpose: Gets a piece from the baord at a given square
// @parameters: Square to get the piece
// @return: None
void Game::selectPieceAt(Squares sq)
{
    currentPiece = board->getPieceAt(sq);
    if (currentPiece != 0)
    {
        // Only select pieces whos turn it is
        if (currentPiece->getColor() != board->fen.activeColor)
        {
            currentPiece = 0;
        }
        else
        {
            currentMoves = currentPiece->getMoves();
        }
    }
}

// @purpose: Processes a Right Click Input
// @parameters: Event to process
// @return: None
void Game::processRightClick(sf::Event event)
{
    // Undo 2 Moves so that it is the Humans Turn again
    board->undoMove();
    board->undoMove();
    result = board->getGameStatus();
    if (result != IN_PROGRESS)
    {
        Logger::log("%s!\n", Chess::EndgameChars[result]);
    }

    if (currentPiece != 0)
    {
        currentPiece = 0;
        currentMoves = 0;
    }
}

// @purpose: Moves a piece to a square and update game state
// @parameters: Piece pointer, Square to mvoe to
// @return: None
void Game::makeMove(Piece* piece, Squares square)
{
    char pawnPromotion = '\0';
    // Handle Pawn promotion
    if (piece->getCharId() == 'P' &&
        square >= A8 && square <= H8)
    {
        pawnPromotion = drawer->getPawnPromotionOption(board->fen.activeColor);
    }
    else if (piece->getCharId() == 'p' &&
        square >= A1 && square <= H1)
    {
        pawnPromotion = tolower(drawer->getPawnPromotionOption(board->fen.activeColor));
    }  

    // Find the Move in the Move vector
    Move move;
    move.pieceMoved = piece;
    move.start = piece->getPos();
    move.end = square;
    move.targetPromotion = pawnPromotion;

    mvVector* moves = board->getAllMoves(piece->getColor());
    mvVector::iterator it = std::find(moves->begin(), moves->end(), move);
    if (it != moves->end())
    {
        board->makeMove(*it);
        board->getMoveHistory()->back().printMove(true);
    }
}