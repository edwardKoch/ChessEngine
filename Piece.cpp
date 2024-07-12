//-------------------------------------------------------
//
// File: Piece.h
// Author: Edward Koch
// Description: Holds the definition of the Piece Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    03/04/23    Initial Creation of Piece class
//-------------------------------------------------------
#include <cctype>

#include "Logger.h"
#include "Piece.h"

// Default Constructor
Piece::Piece(char idIn, Colors colorIn, Squares posIn)
    : type(NUM_PIECES),
      color(colorIn),
      hasMoved(false),
      position(posIn),
      moves(),
      protecting(),
      pinnedPath()
{
    if (color == WHITE)
    {
        charId = toupper(idIn);
    }
    else
    {
        charId = tolower(idIn);
    }
}

// Default Destructor
Piece::~Piece()
{

}

// @purpose: Get a character representation of this piece
// @parameters: None
// @return: character of this piece
char Piece::getCharId()
{
    return charId;
}

// @purpose: Get the type of this piece
// @parameters: None
// @return: Type of this piece
PieceTypes Piece::getType()
{
    return type;
}

// @purpose: Get the color of this piece
// @parameters: None
// @return: Colors color of the piece
Colors Piece::getColor()
{
    return color;
}

// @purpose: Get the position of this piece
// @parameters: None
// @return: Sqaures position of the piece
Squares Piece::getPos()
{
    return position;
}

// @purpose: Sets the hasMoved flag (used in undoing moves)
// @parameters: boolean if the piece has moved or not
// @return: None
void Piece::setHasMoved(bool moved)
{
    hasMoved = moved;
}

// @purpose: Get the move vector of this piece
// @parameters: None
// @return: Vector of moves
mvVector* Piece::getMoves()
{
    return &moves;
}

// @purpose: Checks if an end square is in this pieces set of moves
// @parameters: Square to move to
// @return: True if it is a valid ending square, false otherwise
bool Piece::isSquareInMoves(Squares sq)
{
    mvVector::iterator it = moves.begin();
    for (it = moves.begin(); it != moves.end(); ++it)
    {
        if (it->end == sq)
        {
            return true;
        }
    }

    return false;
}

// @purpose: Get the squares that this piece is protecting
// @parameters: None
// @return: Vector of squares
sqVector* Piece::getProtecting()
{
    return &protecting;
}

// @purpose: Sets the pinnedPath of this piece
// @parameters: Vector of squares
// @return: None
void Piece::setPin(sqVector pinPath)
{
    pinnedPath = pinPath;
}

// @purpose: Clears the pinnedPath of this piece between each move
// @parameters: None
// @return: None
void Piece::clearPin()
{
    pinnedPath.clear();
}

// @purpose: Make a move to a new position
// @parameters: New Square to move to
// @return: None
void Piece::makeMove(Squares newPos)
{
    // Update Position
    position = newPos;
    hasMoved = true;
}

// @purpose: Prints all possible moves for this piece
// @parameters: None
// @return: None
void Piece::printMoves()
{

    Logger::log("%c at %s: ", charId, Chess::SquareChars[position]);
    mvVector::iterator it = moves.begin();
    for (it; it != moves.end(); ++it)
    {
        Logger::log("%s ", Chess::SquareChars[it->end]);
    }
    Logger::log("\n");
}

// @purpose: Print the Piece details
// @parameters: None
// @return: None
void Piece::printPiece()
{
    Logger::enable();
    Logger::log("CharID: %c\n", charId);
    Logger::log("Type: %i\n", type);
    Logger::log("Color: %i\n", color);
    Logger::log("HasMoved: %i\n", hasMoved);
    Logger::log("Position: %s\n", Chess::SquareChars[position]);
    Logger::restore();
}

// @purpose: Checks if a pseudo legal move is actually legal
// @parameters: Square to move to
// @return: true if the move is legal, false otherwise
bool Piece::isLegalMove(Board* board, Squares potentialMove)
{
    // Check if this piece is pinned
    if (!pinnedPath.empty())
    {
        // Search for the potential move in the pinnedPath
        sqVector::iterator it = std::find(pinnedPath.begin(), pinnedPath.end(), potentialMove);

        // If a potential move will take this piece out of the pinned path, the king would then be in check
        // This is an illegal move
        if (it == pinnedPath.end())
        {
            return false;
        }
    }

    // Check if the king is in Check
    std::vector<sqVector>* checkPaths = board->getCheckPaths(color);

    if (!checkPaths->empty())
    {
        // Check for enPassant removal of check
        // If an enemy pawn moved forward 2 to check the king, 
        // allow en passant, even though the en passant square is not in the check path
        if (type == PAWN &&
            potentialMove == board->getFen()->enPassantTarget)
        {
            // Iterate through all possible check paths
            std::vector<sqVector>::iterator it = checkPaths->begin();

            for (it; it != checkPaths->end(); ++it)
            {
                // Check if the En Passant Target is the piece checking the king
                if (abs(it->front() - potentialMove) == Chess::NumFiles)
                {
                    return true;
                }
            }
        }

        // Iterate through all possible check paths
        std::vector<sqVector>::iterator it = checkPaths->begin();

        for (it; it != checkPaths->end(); ++it)
        {
            // Check if a potential move will block check
            sqVector::iterator jt = std::find(it->begin(), it->end(), potentialMove);
            if (jt == it->end())
            {
                // If there is a check path that this move woud not block, return false
                return false;
            }
        }
    }


    return true;
}

// @purpose: Creates a Move object and saves it to the potential moves
// @parameters: Square index to move to
// @return: None
void Piece::addMove(int potentialMove)
{
    Move move;

    move.pieceMoved = this;
    move.start = position;
    move.end = (Squares)potentialMove;

    moves.push_back(move);
}