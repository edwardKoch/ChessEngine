//-------------------------------------------------------
//
// File: Board.h
// Author: Edward Koch
// Description: Holds the definitions of the Board Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    03/03/23    Initial Creation of Board class
//-------------------------------------------------------
#include <iostream>
#include <vector>

#include "Bishop.h"
#include "Board.h"
#include "ChessTypes.h"
#include "Logger.h"
#include "Pawn.h"
#include "Piece.h"
#include "King.h"
#include "Knight.h"
#include "Queen.h"
#include "Rook.h"

// @purpose: Construct and initalize all pieces on the board
// @parameters: string fen that holds a fen position of a board
// @return: None
Board::Board(const char* fenString)
    : board{ 0 },
      charBoard(new char[Chess::NumRanks * Chess::NumFiles]),
      fen(fenString),
      gameStatus(IN_PROGRESS),
      moveHistory(),
      whiteMoves(),
      blackMoves(),
      whiteProtecting(),
      blackProtecting(),
      whiteCheckPaths(),
      blackCheckPaths()
{
    initializeBoard();
    setCharBoard();
    printBoardToConsole();
    // Do initial calculations for both colors
    calculateAllMoves(WHITE);
    calculateAllMoves(BLACK);
    // Recalculate White moves because initially, black moves were empty
    calculateAllMoves(WHITE);
}

// Default Destructor
Board::~Board()
{

}

// @purpose: Construct and initalize all pieces on the board
// @parameters: None
// @return: None
void Board::initializeBoard()
{
    // Initalize the index to -1 inorder to use pre-increment
    int index = -1;
    int boardIndex = -1;

    // Loop through the entire fen string
    while (fen.piecePlacement[++index] != '\0')
    {
        // Skip all spaces
        if (fen.piecePlacement[index] == ' ')
        {
            continue;
        }

        // Parse the Piece Placement Data
        if (++boardIndex <= H1)
        {
            switch (fen.piecePlacement[index])
            {
            case 'r':
                board[boardIndex] = new Rook(BLACK, (Squares)boardIndex);
                break;

            case 'R':
                board[boardIndex] = new Rook(WHITE, (Squares)boardIndex);
                break;

            case 'n':
                board[boardIndex] = new Knight(BLACK, (Squares)boardIndex);
                break;

            case 'N':
                board[boardIndex] = new Knight(WHITE, (Squares)boardIndex);
                break;

            case 'b':
                board[boardIndex] = new Bishop(BLACK, (Squares)boardIndex);
                break;

            case 'B':
                board[boardIndex] = new Bishop(WHITE, (Squares)boardIndex);
                break;

            case 'q':
                board[boardIndex] = new Queen(BLACK, (Squares)boardIndex);
                break;

            case 'Q':
                board[boardIndex] = new Queen(WHITE, (Squares)boardIndex);
                break;

            case 'k':
                board[boardIndex] = new King(BLACK, (Squares)boardIndex);
                BlackKing = board[boardIndex];
                break;

            case 'K':
                board[boardIndex] = new King(WHITE, (Squares)boardIndex);
                WhiteKing = board[boardIndex];
                break;

            case 'p':
                board[boardIndex] = new Pawn(BLACK, (Squares)boardIndex);
                break;

            case 'P':
                board[boardIndex] = new Pawn(WHITE, (Squares)boardIndex);
                break;

            case '/':
                // Decrement the Board index on a 'newline' character
                boardIndex--;
                break;

            default:
                // Decrement the Board index to ignore the pre-increment from the if check
                boardIndex--;

                // Increment the board index by the number of empty squares
                boardIndex += fen.piecePlacement[index] - 48;
                break;
            }
        }
    }
}

// @purpose: prints the current state of the board to the console
// @parameters: None
// @return: None
void Board::printBoardToConsole()
{
    for (int square = A8; square <= H1; ++square)
    {
        if (square % Chess::NumRanks == 0)
        {
            Logger::log("%c |", Chess::Rank[square / Chess::NumRanks]);
        }
        if (board[square] == 0)
        {
            Logger::log(" -");
        }
        else
        {
            Logger::log(" %c", board[square]->getCharId());
        }
        if ((square + 1) % Chess::NumRanks == 0)
        {
            Logger::log("\n");
        }
    }
    Logger::log("   ");
    for (int file = 0; file < Chess::NumFiles; ++file)
    {
        Logger::log(" %c", Chess::File[file]);
    }
    Logger::log("\n");
}

// @purpose: prints the possible moves of all pieces
// @parameters: Colors turn - which color is moving
// @return: None
void Board::printAllMoves(Colors turn)
{
    for (int square = A8; square <= H1; ++square)
    {
        if (board[square] != 0)
        {
            board[square]->calculateMoves(this, turn);
            board[square]->printMoves();
        }
    }
    Logger::log("\n");
}

// @purpose: Prints the move history of the game
// @parameters: If there should be a newline after every move
// @return: None
void Board::printMoveHistory(bool newline)
{
    Logger::enable();
    for (mvVector::iterator it = moveHistory.begin(); it != moveHistory.end(); ++it)
    {
        it->printMove(false);
        if (it->pieceMoved->getColor() == BLACK && newline)
        {
            Logger::log("\n");
        }
    }
    Logger::log("\n\n");
    Logger::restore();
}

// @purpose: Updates the FEN given the game state
// @parameters: None
// @return: None
void Board::calculateFen()
{
    fen.constructFen(getPiecePlacement());
}

// @purpose: Returns the FEN object of the game state
// @parameters: None
// @return: Fen* FEN representation
Fen* Board::getFen()
{
    return &fen;
}

// @purpose: Returns the status of the game
// @parameters: None
// @return: GameResults status
GameResults Board::getGameStatus()
{
    return gameStatus;
}

// @purpose: Generates the FEN.piecePlacement representation of the Board
// @parameters: None
// @return: const char* FEN representation
const char* Board::getPiecePlacement()
{
    const int MaxFenLength = 71;
    char* piecePlacement = new char[MaxFenLength];

    int emptySquares = 0;
    int fenIndex = 0;

    for (int square = A8; square <= H1; ++square)
    {
        // Place the '/' at the end of each rank
        if (square >= A8 && square % Chess::NumRanks == 0)
        {
            if (emptySquares > 0)
            {
                // Place the number of empty squares (as a char) into the FEN
                piecePlacement[fenIndex++] = emptySquares + 48;
                // Reset the number of empty squares
                emptySquares = 0;
            }
            piecePlacement[fenIndex++] = '/';
        }

        // Count the number of consecutive empty squares
        if (board[square] == 0)
        {
            emptySquares++;

            // Handle the final character
            if (square == H1)
            {
                // Place the number of empty squares (as a char) into the FEN
                piecePlacement[fenIndex++] = emptySquares + 48;
                // Reset the number of empty squares
                emptySquares = 0;
            }
        }
        // If a piece is found after a number of empty squares
        else if (board[square] != 0 && emptySquares > 0)
        {
            // Place the number of empty squares (as a char) into the FEN
            piecePlacement[fenIndex++] = emptySquares + 48;
            // Reset the number of empty squares
            emptySquares = 0;
        }
        
        // If a piece is found
        if (board[square] != 0)
        {
            // Place the piece (as a char) into the FEN
            piecePlacement[fenIndex++] = board[square]->getCharId();
        }
    }    

    // Fill the rest of the string with Null Characters
    while (++fenIndex < MaxFenLength)
    {
        piecePlacement[fenIndex] = '\0';
    }
    
    return piecePlacement;
}

// @purpose: Gets the Fen Representation of the board state
// @parameters: None
// @return: const char* char representation
const char* Board::getFenString()
{
    fen.constructFen(getPiecePlacement());
    return fen.fen;
}

// @purpose: Gets the 64-length character array that represents the board state
// @parameters: None
// @return: const char* char representation
const char* Board::getCharBoard()
{
    return charBoard;
}

// @purpose: Generates a 64-length character array that represents the board state
// @parameters: None
// @return: const char* char representation
void Board::setCharBoard()
{
    for (int square = A8; square <= H1; ++square)
    {
        if (board[square] == 0)
        {
            charBoard[square] = '-';
        }
        else
        {
            charBoard[square] = board[square]->getCharId();
        }
    }
}

// @purpose: Tells the board that a king is in check from a piece
// @parameters: Path from checker to king, color of king
// @return: None
void Board::addCheckPath(sqVector checkPath, Colors king)
{
    if (king == WHITE)
    {
        whiteCheckPaths.push_back(checkPath);
    }
    else
    {
        blackCheckPaths.push_back(checkPath);
    }
}

// @purpose: Gets all of the check paths from an enemy to a king
// @parameters: Color of king
// @return: vector of paths from which the king is in check
std::vector<sqVector>* Board::getCheckPaths(Colors king)
{
    if (king == WHITE)
    {
        return &whiteCheckPaths;
    }
    else
    {
        return &blackCheckPaths;
    }
}

// @purpose: Get a reference to a piece on the board
// @parameters: Square where the piece is
// @return: Piece reference
Piece* Board::getPieceAt(Squares pos)
{
    return board[pos];
}

// @purpose: Get a reference to the king of a given color
// @parameters: Color of the King
// @return: Piece reference
Piece* Board::getKing(Colors color)
{
    if (color == WHITE)
    {
        return WhiteKing;
    }
    else
    {
        return BlackKing;
    }
}

// @purpose: Calculates all moves of all pieces of one color
// @parameters: Color of the pieces to calculate moves for
// @return: None
void Board::calculateAllMoves(Colors pieceColor)
{
    sqVector* tmp;
    mvVector* tmpMv = 0;
    mvVector* moves = 0;
    sqVector* protects = 0;

    // Only Update the moves array of the current color
    if (pieceColor == WHITE)
    {
        moves = &whiteMoves;
        protects = &whiteProtecting;
    }
    else
    {
        moves = &blackMoves;
        protects = &blackProtecting;
    }

    moves->clear();
    protects->clear();

    for (int square = A8; square <= H1; square++)
    {
        if (board[square] == 0) continue;

        if (board[square]->getColor() == pieceColor)
        {
            // Calculate moves for a piece
            board[square]->calculateMoves(this, pieceColor);

            // Get the Moves
            tmpMv = board[square]->getMoves();
            moves->insert(moves->end(), tmpMv->begin(), tmpMv->end());
            tmpMv = 0;

            // Get the squares that the piece is protecting
            tmp = board[square]->getProtecting();
            protects->insert(protects->end(), tmp->begin(), tmp->end());
            tmp = 0;
        }
    }
}

// @purpose: Get all moves of all pieces of one color
// @parameters: Color of the pieces to calculate moves for
// @return: vector of all moves that the color can move to
mvVector* Board::getAllMoves(Colors pieceColor)
{
    if (pieceColor == WHITE)
    {
        return &whiteMoves;
    }
    else
    {
        return &blackMoves;
    }
}

// @purpose: Get the move hisotry for the game
// @parameters: None
// @return: Vector of moves that have been played
mvVector* Board::getMoveHistory()
{
    return &moveHistory;
}

// @purpose: Get all squares that one piece is protecting
// @parameters: Color of the pieces to calculate moves for
// @return: vector of all squares that the color can move to
sqVector* Board::getAllProtected(Colors pieceColor)
{
    if (pieceColor == WHITE)
    {
        return &whiteProtecting;
    }
    else
    {
        return &blackProtecting;
    }
}

//@purpose: Moves a piece from one square to another
// @parameters: Move object with all move information
// @return: None
void Board::makeMove(Move move)
{
    // Track the move history
    move.oldFen = fen;

    // If the end square is not empty, capture that piece
    if (board[move.end] != 0)
    {
        move.pieceCaptured = board[move.end];
        board[move.end] = 0;

        // Reset the Halfmove Clock
        fen.halfmoveClock = 0;
    }

    // Handle En Passant
    checkEnPassant(&move);

    // Handle Castling
    checkCastling(&move);

    // Move the piece
    movePiece(move.start, move.end);

    // Handle Pawn Promotion
    if (move.targetPromotion != '\0')
    {
        promotePawn(&move);
    }

    // Log the Move in the Move History
    moveHistory.push_back(move);

    // Update the Character Representation of the Board
    setCharBoard();

    // Clear all Pins and Checks before re-calculating moves
    clearAllConditions();

    // Update the possible moves of the color that just moved
    calculateAllMoves(fen.activeColor);

    // Update which color's turn it is
    fen.activeColor = Chess::enemy[fen.activeColor];

    // Update the possible moves of the color who will now move
    calculateAllMoves(fen.activeColor);

    // Update the Fen
    calculateFen();

    // Check for the EndGame
    processEndgame();
}

// @purpose: Un-makes the last move
// @parameters: None
// @return: None
void Board::undoMove()
{
    // Check if there are no moves to undo
    if (moveHistory.empty())
    {
        return;
    }
    // Get the last move
    Move lastMove = moveHistory.back();
    moveHistory.pop_back();

    // Undo a Pawn Promotion
    if (lastMove.piecePromoted != 0)
    {
        // Delete the new piece
        delete board[lastMove.end];
        lastMove.piecePromoted = 0;

        // Restore the Pawn - in the final rank
        board[lastMove.end] = lastMove.pieceMoved;
    }

    // Move the piece back to its starting square
    movePiece(lastMove.end, lastMove.start);

    // Restore a captured Piece
    if (lastMove.pieceCaptured != 0)
    {
        board[lastMove.pieceCaptured->getPos()] = lastMove.pieceCaptured;
    }

    // Restore Castling
    if (lastMove.pieceCastled != 0)
    {
        // Clear the space where the Rook moved to
        board[lastMove.pieceCastled->getPos()] = 0;
        // Restore the position of the Rook
        lastMove.pieceCastled->makeMove(lastMove.castledFrom);
        board[lastMove.castledFrom] = lastMove.pieceCastled;

        // Restore the 'hasMoved' flags that are used in calculating Castling rights
        lastMove.pieceMoved->setHasMoved(false);
        lastMove.pieceCastled->setHasMoved(false);
    }

    // Retore the Fen
    fen = lastMove.oldFen;

    // Update the Character Representation of the Board
    setCharBoard();

    // Clear all Pins and Checks before re-calculating moves
    clearAllConditions();

    // Recalculate the Enemies moves
    calculateAllMoves(Chess::enemy[fen.activeColor]);

    // Recalculate the colors moves who undid their turn
    calculateAllMoves(fen.activeColor);

    // Check for the EndGame
    processEndgame();
}

// @purpose: Checks for an ending condition
// @parameters: None
// @return: Enumeration of game ends
GameResults Board::checkEndgame()
{
    mvVector* moves = getAllMoves(fen.activeColor);
    std::vector<sqVector>* checkPaths = getCheckPaths(fen.activeColor);

    // Check if there are no legal moves
    if (moves->empty())
    {
        // Check if the current color is in check
        if (!checkPaths->empty())
        {
            // CheckMate
            if (fen.activeColor == WHITE)
            {
                // Return opposite color win, because we are checking after turn switch
                return BLACK_WIN;
            }
            else
            {
                return WHITE_WIN;
            }
        }

        // If there are no moves, but also no check, then it is a Stalemate
        return DRAW_STALEMATE;
    }

    // Check for Draw by 50 moves (100 halfmoves)
    if (fen.halfmoveClock >= 100)
    {
        return DRAW_50MOVES;
    }

    // Check for 3-fold repetition
    int threeFold = 0;
    mvVector::iterator it = moveHistory.end();
    while (it != moveHistory.begin())
    {
        --it;

        // Captures, Pawn Moves, and Castles reset the repetition rule
        if (it->pieceMoved->getType() == PAWN ||
            it->pieceCastled != 0 ||
            it->pieceCaptured != 0)
        {
            break;
        }

        // If the piece placement and castling rights are the same, count a repetition
        if (strcmp(fen.piecePlacement,it->oldFen.piecePlacement) == 0 &&
            fen.activeColor == it->oldFen.activeColor &&
            fen.enPassantTarget == it->oldFen.enPassantTarget &&
            fen.kingCastleWhite == it->oldFen.kingCastleWhite &&
            fen.queenCastleWhite == it->oldFen.queenCastleWhite &&
            fen.kingCastleBlack == it->oldFen.kingCastleBlack &&
            fen.queenCastleBlack == it->oldFen.queenCastleBlack)
        {
            if (++threeFold >= 3)
            {
                return DRAW_REPETITION;
            }
        }
    }

    return IN_PROGRESS;
}

// @purpose: Moves a piece from one square to another
// @parameters: Square start position, Square end position
// @return: None
void Board::movePiece(Squares start, Squares end)
{
    // Check if the moving piece is a pawn
    if (tolower(board[start]->getCharId()) == 'p')
    {
        // Reset the Halfmove Clock
        fen.halfmoveClock = 0;
    }
    // Move the piece from the starting positon to the ending position
    board[start]->makeMove(end);
    board[end] = board[start];
    board[start] = 0;

    // Increment the halfmove clock after every move
    fen.halfmoveClock += 1;

    // Increment the Full move clock after every Block moves
    if (fen.activeColor == BLACK)
    {
        fen.fullmoveNumber += 1;
    }   
}

// @purpose: Handle all En Passant Actions
// @parameters: Move object to update
// @return: None
void Board::checkEnPassant(Move* move)
{
    // Handle En Passant Captures
    if (move->end == fen.enPassantTarget)
    {
        if (fen.activeColor == WHITE &&
            move->pieceMoved->getCharId() == 'P')
        {
            move->pieceCaptured = board[(move->end + 8)];
            board[(move->end + 8)] = 0;
        }
        else if (move->pieceMoved->getCharId() == 'p')
        {
            move->pieceCaptured = board[(move->end - 8)];
            board[(move->end - 8)] = 0;
        }
    }

    // Track En Passant Squares
    if (move->pieceMoved->getCharId() == 'p' &&
        abs(move->start - move->end) == 16)
    {
        fen.enPassantTarget = (Squares)(move->end - 8);
    }
    else if (move->pieceMoved->getCharId() == 'P' &&
        abs(move->start - move->end) == 16)
    {
        fen.enPassantTarget = (Squares)(move->end + 8);
    }
    else
    {
        fen.enPassantTarget = INVALID;
    }
}

// @purpose: Handle all Castling Actions
// @parameters: Move object to update
// @return: None
void Board::checkCastling(Move* move)
{
    // Handle White Castling
    if (move->pieceMoved->getCharId() == 'K')
    {
        // If a King is Moving, revoke castling rights
        fen.kingCastleWhite = false;
        fen.queenCastleWhite = false;

        // Kingside Castling
        if (move->start == E1 && move->end == G1)
        {
            move->pieceCastled = board[H1];
            move->castledFrom = H1;
            movePiece(H1, F1);
        }
        // Queenside Castling
        else if (move->start == E1 && move->end == C1)
        {
            move->pieceCastled = board[A1];
            move->castledFrom = A1;
            movePiece(A1, D1);
        }
    }

    // Handle Black Castling
    if (move->pieceMoved->getCharId() == 'k')
    {
        // If a King is Moving, revoke castling rights
        fen.kingCastleBlack = false;
        fen.queenCastleBlack = false;

        // Kingside Castling
        if (move->start == E8 && move->end == G8)
        {
            move->pieceCastled = board[H8];
            move->castledFrom = H8;
            movePiece(H8, F8);
        }
        // Queenside Castling
        else if (move->start == E8 && move->end == C8)
        {
            move->pieceCastled = board[A8];
            move->castledFrom = A8;
            movePiece(A8, D8);
        }
    }

    // Revoke Castling Rights on Rook Move
    if (move->pieceMoved->getCharId() == 'R')
    {
        if (move->start == A1)
        {
            fen.queenCastleWhite = false;
        }
        else if (move->start == H1)
        {
            fen.kingCastleWhite = false;
        }
    }
    else if (move->pieceMoved->getCharId() == 'r')
    {
        if (move->start == A8)
        {
            fen.queenCastleBlack = false;
        }
        else if (move->start == H8)
        {
            fen.kingCastleBlack = false;
        }
    }
}

// @purpose: Promotes a Pawn that reaches the end of the board
// @parameters: Move object to update
// @return: None
void Board::promotePawn(Move* move)
{
    // Remove the pawn from the board
    board[move->end] = 0;

    // Replace the Pawn with the Desired piece
    switch (move->targetPromotion)
    {
    case 'r':
        board[move->end] = new Rook(BLACK, move->end);
        break;

    case 'R':
        board[move->end] = new Rook(WHITE, move->end);
        break;

    case 'n':
        board[move->end] = new Knight(BLACK, move->end);
        break;

    case 'N':
        board[move->end] = new Knight(WHITE, move->end);
        break;

    case 'b':
        board[move->end] = new Bishop(BLACK, move->end);
        break;

    case 'B':
        board[move->end] = new Bishop(WHITE, move->end);
        break;

    case 'q':
        board[move->end] = new Queen(BLACK, move->end);
        break;

    case 'Q':
        board[move->end] = new Queen(WHITE, move->end);
        break;
    }

    move->piecePromoted = board[move->end];

    // Update the Character Representation of the Board
    setCharBoard();
}

// @purpose: Clears the pinnedPath from all pieces
// @parameters: None
// @return: None
void Board::clearAllConditions()
{
    // Clear the check paths
    whiteCheckPaths.clear();
    blackCheckPaths.clear();

    // Clear all pin paths
    for (int square = A8; square <= H1; square++)
    {
        if (board[square] == 0) continue;

        board[square]->clearPin();
    }
}

// @purpose: Processes all Endgame functionality
// @parameters: None
// @return: None
void Board::processEndgame()
{
    // Check if there is an endgame condition
    gameStatus = checkEndgame();
    if (gameStatus == IN_PROGRESS) return;

    //Logger::log("%s!\n", Chess::EndgameChars[gameStatus]);
}