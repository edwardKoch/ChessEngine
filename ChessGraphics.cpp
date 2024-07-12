//-------------------------------------------------------
//
// File: Game.h
// Author: Edward Koch
// Description: Holds the definition of the ChessGraphics Class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    03/07/23    Initial Creation of ChessGraphics class
//-------------------------------------------------------
#include "Board.h"
#include "ChessGraphics.h"
#include "Logger.h"
#include "Move.h"
#include "Piece.h"

// Default Constructor
ChessGraphics::ChessGraphics()
    : windowSize(800),
      squareSize((float)windowSize / Chess::NumFiles),
      titleFontSize(squareSize / 3),
      wordFontSize(squareSize / 4),
      window(0)
{
    titleFontSize = squareSize / 3;
    wordFontSize = squareSize / 4;

    window = new sf::RenderWindow(sf::VideoMode(windowSize, windowSize), "Chess");
    if (!font.loadFromFile("resources/arial.ttf"))
    {
        Logger::log("YIKES NO FONT\n");
    }

    prepareBoard();
    preparePieces();
}

// Default Destructor
ChessGraphics::~ChessGraphics()
{

}

// @purpose: Coverts Mouse coordinates to a Chess Square
// @parameters: Mouse Event
// @return: Chess Square
Squares ChessGraphics::mouseToSquare(sf::Event event)
{
    int row = event.mouseButton.y / squareSize;
    int col = event.mouseButton.x / squareSize;

    int square = row * Chess::NumFiles + col;

    return (Squares)square;
}

// @purpose: Coverts Mouse coordinates to a Chess Square
// @parameters: Mouse Event
// @return: Chess Square
sf::Vector2f ChessGraphics::squareToMouse(Squares sq)
{
    int y = (sq / Chess::NumFiles);
    int x = sq - (y * Chess::NumFiles);

    return sf::Vector2f(x * squareSize, y * squareSize);
}

// @purpose: Draws the Chess Board and Pieces
// @parameters: Board Object, Current piece to draw around
// @return: None
void ChessGraphics::drawGame(Board* board, Piece* currentPiece)
{
    window->clear();
    drawBoard();
    drawHighlights(board, currentPiece);
    drawMarkings();
    drawPieces(board->getCharBoard());
    window->display();
}

// @purpose: Prepares base board for drawing
// @parameters: None
// @return: None
void ChessGraphics::prepareBoard()
{
    // Prepard Board Squares
    sf::Color darkSquare = sf::Color(147, 82, 51);
    sf::Color lightSquare = sf::Color(218, 184, 145);

    float xPos = 0;
    float yPos = 0;
    int rankNum = 1;
    for (int i = 0; i < Chess::NumSquares; ++i)
    {
        if (i > 0)
        {
            if (i % 8 == 0)
            {
                xPos = 0;
                yPos += squareSize;
                rankNum++;
            }
            else
            {
                xPos += squareSize;
            }
        }
        
        // Initialize Game Board
        squares[i] = sf::RectangleShape(sf::Vector2f(squareSize, squareSize));
        squares[i].setPosition(sf::Vector2f(xPos, yPos));
        squares[i].setOutlineColor(sf::Color(0, 0, 0));
        squares[i].setOutlineThickness(1.0f);
        if ((i + rankNum) % 2 == 1)
        {
            squares[i].setFillColor(lightSquare);
        }
        else
        {
            squares[i].setFillColor(darkSquare);
        }

        // Initialize Highlighting Squares
        highlightSquares[i] = sf::RectangleShape(sf::Vector2f(squareSize, squareSize));
        highlightSquares[i].setPosition(sf::Vector2f(xPos, yPos));
        highlightSquares[i].setOutlineColor(sf::Color(0, 0, 0));
        highlightSquares[i].setOutlineThickness(1.0f);
    }
}

// @purpose: Prepares the piece images
// @parameters: None
// @return: None
void ChessGraphics::preparePieces()
{
    whiteKing.loadFromFile("resources/WhiteKing.png");
    blackKing.loadFromFile("resources/BlackKing.png");
    whiteQueen.loadFromFile("resources/WhiteQueen.png");
    blackQueen.loadFromFile("resources/BlackQueen.png");
    whiteRook.loadFromFile("resources/WhiteRook.png");
    blackRook.loadFromFile("resources/BlackRook.png");
    whiteBishop.loadFromFile("resources/WhiteBishop.png");
    blackBishop.loadFromFile("resources/BlackBishop.png");
    whiteKnight.loadFromFile("resources/WhiteKnight.png");
    blackKnight.loadFromFile("resources/BlackKnight.png");
    whitePawn.loadFromFile("resources/WhitePawn.png");
    blackPawn.loadFromFile("resources/BlackPawn.png");

    // Create all of the Sprites
    float xPos = 0;
    float yPos = 0;
    int rankNum = 1;
    for (int i = 0; i < Chess::NumSquares; ++i)
    {
        if (i > 0)
        {
            if (i % 8 == 0)
            {
                xPos = 0;
                yPos += squareSize;
                rankNum++;
            }
            else
            {
                xPos += squareSize;
            }
        }

        pieceSprites[i] = new sf::Sprite();
        pieceSprites[i]->setPosition(sf::Vector2f(xPos, yPos));
    }
}

// @purpose: Draws the squares of the chess board
// @parameters: None
// @return: None
void ChessGraphics::drawBoard()
{

    for (int i = 0; i < Chess::NumSquares; ++i)
    {
        window->draw(squares[i]);
    }
}

// @purpose: Draws the rank and file markings on the board
// @parameters: None
// @return: None
void ChessGraphics::drawMarkings()
{
    // Draw Rank and File Markings
    int fontSize = 15;

    // reset positions
    float xPos = 0;
    float yPos = 0;

    for (int i = 0; i < Chess::NumRanks; ++i)
    {
        rankMarks[i] = sf::Text(Chess::Rank[i], font, fontSize);
        rankMarks[i].setPosition(sf::Vector2f(xPos, yPos));
        rankMarks[i].setFillColor(sf::Color::Black);
        rankMarks[i].setStyle(sf::Text::Bold);

        yPos += squareSize;

        window->draw(rankMarks[i]);
    }

    // reset positions
    xPos = -fontSize;
    yPos = windowSize - fontSize;

    for (int i = 0; i < Chess::NumFiles; ++i)
    {
        xPos += squareSize;
        fileMarks[i] = sf::Text(Chess::File[i], font, fontSize);
        fileMarks[i].setPosition(sf::Vector2f(xPos, yPos));
        fileMarks[i].setFillColor(sf::Color::Black);
        fileMarks[i].setStyle(sf::Text::Bold);

        window->draw(fileMarks[i]);
    }
}

// @purpose: Draws the Peices based on the board
// @parameters: Character representation of the board
// @return: None
void ChessGraphics::drawPieces(const char* board)
{
    for (int i = 0; i < Chess::NumSquares; ++i)
    {
        switch (board[i])
        {
        case 'r':
            pieceSprites[i]->setTexture(blackRook);
            break;

        case 'R':
            pieceSprites[i]->setTexture(whiteRook);
            break;

        case 'n':
            pieceSprites[i]->setTexture(blackKnight);
            break;

        case 'N':
            pieceSprites[i]->setTexture(whiteKnight);
            break;

        case 'b':
            pieceSprites[i]->setTexture(blackBishop);
            break;

        case 'B':
            pieceSprites[i]->setTexture(whiteBishop);
            break;

        case 'q':
            pieceSprites[i]->setTexture(blackQueen);
            break;

        case 'Q':
            pieceSprites[i]->setTexture(whiteQueen);
            break;

        case 'k':
            pieceSprites[i]->setTexture(blackKing);
            break;

        case 'K':
            pieceSprites[i]->setTexture(whiteKing);
            break;

        case 'p':
            pieceSprites[i]->setTexture(blackPawn);
            break;

        case 'P':
            pieceSprites[i]->setTexture(whitePawn);
            break;

        default:
            continue;
            break;
        }

        sf::Vector2f spriteSize(squareSize, squareSize);

        pieceSprites[i]->setScale(
            spriteSize.x / pieceSprites[i]->getLocalBounds().width,
            spriteSize.y / pieceSprites[i]->getLocalBounds().height);

        window->draw(*pieceSprites[i]);
    }
}

// @purpose: Draws highlighted squares for potential moves
// @parameters: Board Object, Current piece to draw around
// @return: None
void ChessGraphics::drawHighlights(Board* board, Piece* currentPiece)
{
    // Highlight the last move
    mvVector* moveHistory = board->getMoveHistory();
    if (!moveHistory->empty())
    {
        Move lastMove = moveHistory->back();
        sf::Color lastMoveColor = sf::Color(246, 246, 104);
        highlightSquares[lastMove.start].setFillColor(lastMoveColor);
        window->draw(highlightSquares[lastMove.start]);

        highlightSquares[lastMove.end].setFillColor(lastMoveColor);
        window->draw(highlightSquares[lastMove.end]);
    }

    // Draw colors around a checked king
    std::vector<sqVector>* checkPaths = board->getCheckPaths(board->getFen()->activeColor);
    if (!checkPaths->empty())
    {
        sf::Color checkedKing = sf::Color(212, 41, 41);
        sf::Color checkingPiece = sf::Color(255, 150, 150);

        // Iterate through all possible check paths
        std::vector<sqVector>::iterator it = checkPaths->begin();

        for (it; it != checkPaths->end(); ++it)
        {
            highlightSquares[it->front()].setFillColor(checkingPiece);
            window->draw(highlightSquares[it->front()]);

            highlightSquares[it->back()].setFillColor(checkedKing);
            window->draw(highlightSquares[it->back()]);
        }
    }

    // Highlight the current piece, if there is one
    if (currentPiece == 0) return;

    sf::Color currentPieceColor = sf::Color(50, 225, 75);
    highlightSquares[currentPiece->getPos()].setFillColor(currentPieceColor);
    window->draw(highlightSquares[currentPiece->getPos()]);

    const char* charBoard = board->getCharBoard();
    mvVector* moves = currentPiece->getMoves();

    // Draw all current moves
    if (!moves->empty())
    {
        int highlightIndex = 0;

        sf::Color emptySquare = sf::Color(50, 180, 225);
        sf::Color takenSquare = sf::Color(255, 50, 50);

        mvVector::iterator it = moves->begin();

        for (it = moves->begin(); it != moves->end(); ++it)
        {
            if (charBoard[it->end] == '-')
            {
                highlightSquares[it->end].setFillColor(emptySquare);
            }
            else
            {
                highlightSquares[it->end].setFillColor(takenSquare);
            }
            
            window->draw(highlightSquares[it->end]);
        }
    }
}

// @purpose: Display options for the user to promote a Pawn to
// @parameters: Color of Piece to pick
// @return: Character representation of the piece to promote to
char ChessGraphics::getPawnPromotionOption(Colors color)
{
    sf::Color background;
    sf::Texture* queen = 0;
    sf::Texture* rook = 0;
    sf::Texture* bishop = 0;
    sf::Texture* knight = 0;
    Squares displaySquares[4] = {INVALID, INVALID, INVALID, INVALID};

    if (color == WHITE)
    {
        background = sf::Color(0,0,0);
        queen = &whiteQueen;
        rook = &whiteRook;
        bishop = &whiteBishop;
        knight = &whiteKnight;
        displaySquares[0] = C5;
        displaySquares[1] = D5;
        displaySquares[2] = E5;
        displaySquares[3] = F5;
    }
    else
    {
        background = sf::Color(255, 255, 255);
        queen = &blackQueen;
        rook = &blackRook;
        bishop = &blackBishop;
        knight = &blackKnight;
        displaySquares[0] = C4;
        displaySquares[1] = D4;
        displaySquares[2] = E4;
        displaySquares[3] = F4;
    }

    // Draw Background for Selection
    for (int i = 0; i < 4; ++i)
    {
        int index = displaySquares[i];
        highlightSquares[index].setFillColor(background);
        window->draw(highlightSquares[index]);
    }

    // Draw Sprites to Choose From
    int spriteIndex = 0;
    pieceSprites[displaySquares[spriteIndex++]]->setTexture(*queen);
    pieceSprites[displaySquares[spriteIndex++]]->setTexture(*rook);
    pieceSprites[displaySquares[spriteIndex++]]->setTexture(*bishop);
    pieceSprites[displaySquares[spriteIndex++]]->setTexture(*knight);
    
    sf::Vector2f spriteSize(squareSize, squareSize);
    for (int i = 0; i < 4; ++i)
    {
        int index = displaySquares[i];

        pieceSprites[index]->setScale(
            spriteSize.x / pieceSprites[index]->getLocalBounds().width,
            spriteSize.y / pieceSprites[index]->getLocalBounds().height);

        window->draw(*pieceSprites[index]);
    }

    window->display();

    // Get User Selection
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    Squares sq = mouseToSquare(event);

                    if (sq == displaySquares[0])
                    {
                        return 'Q';
                    }
                    else if (sq == displaySquares[1])
                    {
                        return 'R';
                    }
                    else if (sq == displaySquares[2])
                    {
                        return 'B';
                    }
                    else if (sq == displaySquares[3])
                    {
                        return 'N';
                    }
                }
            }
        }
    }

    return 'Q';

}

// @purpose: Draws the Starting Menu
// @parameters: None
// @return: None
void ChessGraphics::drawMainMenu()
{
    window->clear();
    //drawBoard(); // DEBUG

    float xPos = 0;
    float yPos = 0;

    // Draw Title
    sf::String titleStr = "Chess!";
    sf::Text title("Chess!", font, titleFontSize);
    xPos = (windowSize/2) - ((titleStr.getSize() / 2.0) * title.getCharacterSize())/2;
    yPos = squareSize * 2;
    title.setPosition(xPos, yPos);
    window->draw(title);

    // Draw Game Modes

    sf::String pvpStr = "Human vs Human";
    sf::Text pvp(pvpStr, font, wordFontSize);
    xPos = (windowSize / 2) - ((pvpStr.getSize() / 2.0) * pvp.getCharacterSize()) / 2;
    yPos += squareSize + squareSize / 3;
    pvp.setPosition(xPos, yPos);
    window->draw(pvp);

    sf::String pvcStr = "Human vs Computer";
    sf::Text pvc(pvcStr, font, wordFontSize);
    xPos = (windowSize / 2) - ((pvcStr.getSize() / 2.0) * pvc.getCharacterSize()) / 2;
    yPos += squareSize;
    pvc.setPosition(xPos, yPos);
    window->draw(pvc);

    sf::String cvcStr = "Computer vs Computer";
    sf::Text cvc(cvcStr, font, wordFontSize);
    xPos = (windowSize / 2) - ((cvcStr.getSize() / 2.0) * cvc.getCharacterSize()) / 2;
    yPos += squareSize;
    cvc.setPosition(xPos, yPos);
    window->draw(cvc);

    window->display();
}

// @purpose: Draws the Chess Board and Pieces
// @parameters: Game Mode, Color Selection
// @return: None
void ChessGraphics::drawPvcMenu(Colors human, int depth)
{
    window->clear();
    //drawBoard(); // DEBUG

    float xPos = 0;
    float yPos = 0;

    // Draw Title
    sf::Text title("Human Color", font, titleFontSize);
    xPos = squareSize * 3;// +(float)title.getCharacterSize();
    yPos = squareSize * 2;
    title.setPosition(xPos, yPos);
    window->draw(title);

    // Draw Color Choice
    sf::Text white("White", font, wordFontSize);
    xPos = squareSize * 3;
    yPos += squareSize + squareSize / 3;
    white.setPosition(xPos, yPos);
    if (human == WHITE)
    {
        white.setOutlineColor(sf::Color::Cyan);
        white.setOutlineThickness(4);
    }
    window->draw(white);

    sf::Text black("Black", font, wordFontSize);
    xPos = squareSize * 4 + (float)black.getCharacterSize();
    yPos = yPos;
    black.setPosition(xPos, yPos);
    if (human == BLACK)
    {
        black.setOutlineColor(sf::Color::Cyan);
        black.setOutlineThickness(4);
    }
    window->draw(black);

    sf::Text aiStrength("AI Strength", font, wordFontSize);
    xPos = squareSize * 3.5;
    yPos += squareSize;
    aiStrength.setPosition(xPos, yPos);
    window->draw(aiStrength);

    xPos = squareSize/2;
    yPos += squareSize ;
    // Draw AI Strength Options
    for (int i = 1; i < 7; ++i)
    {
        char strenChar = '0' + i;
        sf::Text aiStrength(strenChar, font, wordFontSize);
        xPos += squareSize;
        if (i == depth)
        {
            aiStrength.setOutlineColor(sf::Color::Cyan);
            aiStrength.setOutlineThickness(4);
        }
        aiStrength.setPosition(xPos, yPos);
        window->draw(aiStrength);
    }

    sf::Text start("START", font, wordFontSize);
    xPos = squareSize * 3.6;
    yPos += squareSize;
    start.setPosition(xPos, yPos);
    window->draw(start);

    window->display();
}

// @purpose: Draws the Chess Board and Pieces
// @parameters: Game Mode, Color Selection
// @return: None
void ChessGraphics::drawCvcMenu(int whiteDepth, int blackDepth)
{
    window->clear();
    //drawBoard(); // DEBUG

    float xPos = 0;
    float yPos = 0;

    // White AI Strength Options
    sf::Text whiteAi("White Strength", font, wordFontSize);
    xPos = squareSize * 3.3;
    yPos += squareSize + squareSize/3;
    whiteAi.setPosition(xPos, yPos);
    window->draw(whiteAi);

    xPos = squareSize / 2;
    yPos += squareSize;
    // Draw White AI Strength Options
    for (int i = 1; i < 7; ++i)
    {
        char strenChar = '0' + i;
        sf::Text aiStrength(strenChar, font, wordFontSize);
        xPos += squareSize;
        if (i == whiteDepth)
        {
            aiStrength.setOutlineColor(sf::Color::Cyan);
            aiStrength.setOutlineThickness(4);
        }
        aiStrength.setPosition(xPos, yPos);
        window->draw(aiStrength);
    }

    // Black AI Strength Options
    sf::Text blackAi("Black Strength", font, wordFontSize);
    xPos = squareSize * 3.3;
    yPos += squareSize;
    blackAi.setPosition(xPos, yPos);
    window->draw(blackAi);

    xPos = squareSize / 2;
    yPos += squareSize;
    // Draw Black AI Strength Options
    for (int i = 1; i < 7; ++i)
    {
        char strenChar = '0' + i;
        sf::Text aiStrength(strenChar, font, wordFontSize);
        xPos += squareSize;
        if (i == blackDepth)
        {
            aiStrength.setOutlineColor(sf::Color::Cyan);
            aiStrength.setOutlineThickness(4);
        }
        aiStrength.setPosition(xPos, yPos);
        window->draw(aiStrength);
    }

    sf::Text start("START", font, wordFontSize);
    xPos = squareSize * 3.6;
    yPos += squareSize * 2;
    start.setPosition(xPos, yPos);
    window->draw(start);

    window->display();
}