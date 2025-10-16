#include "TicTacToe.h"
#include "Logger.h"

// -----------------------------------------------------------------------------
// TicTacToe.cpp
// -----------------------------------------------------------------------------
// This file is intentionally *full of comments* and gentle TODOs that guide you
// through wiring up a complete Tic‑Tac‑Toe implementation using the game engine’s
// Bit / BitHolder grid system.
//
// Rules recap:
//  - Two players place X / O on a 3x3 grid.
//  - Players take turns; you can only place into an empty square.
//  - First player to get three-in-a-row (row, column, or diagonal) wins.
//  - If all 9 squares are filled and nobody wins, it’s a draw.
//
// Notes about the provided engine types you'll use here:
//  - Bit              : a visual piece (sprite) that belongs to a Player
//  - BitHolder        : a square on the board that can hold at most one Bit
//  - Player           : the engine’s player object (you can ask who owns a Bit)
//  - Game options     : let the mouse know the grid is 3x3 (rowX, rowY)
//  - Helpers you’ll see used: setNumberOfPlayers, getPlayerAt, startGame, etc.
//
// I’ve already fully implemented PieceForPlayer() for you. Please leave that as‑is.
// The rest of the routines are written as “comment-first” TODOs for you to complete.
// -----------------------------------------------------------------------------

//const int AI_PLAYER   = 1;      // index of the AI player (O)
//const int HUMAN_PLAYER= 0;      // index of the human player (X)

Logger &logger = Logger::GetInstance();
bool gameOver = false;

TicTacToe::TicTacToe()
{
}

TicTacToe::~TicTacToe()
{
}

// -----------------------------------------------------------------------------
// make an X or an O
// -----------------------------------------------------------------------------
// DO NOT CHANGE: This returns a new Bit with the right texture and owner
Bit* TicTacToe::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    bit->LoadTextureFromFile(playerNumber == 0 ? "x.png" : "o.png");
    bit->setOwner(getPlayerAt(playerNumber));
    return bit;
}

//
// setup the game board, this is called once at the start of the game
//
void TicTacToe::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 3;
    _gameOptions.rowY = 3;
    
    // Fill board with squares
    int xOffset = 25, yOffset = 25;
    for (int rowX = 0; rowX < _gameOptions.rowX; rowX++) 
    {
        for (int rowY = 0; rowY < _gameOptions.rowY; rowY++) 
        {
            _grid[rowX][rowY].initHolder(ImVec2(rowX * 100 + xOffset, rowY * 100 + yOffset), "square.png", rowX, rowY);
        }
    }

    startGame();
}

//
// about the only thing we need to actually fill out for tic-tac-toe
//
bool TicTacToe::actionForEmptyHolder(BitHolder *holder)
{
    if (gameOver) return false;
    if (!holder) return false;
    if (holder->bit()) return false;

    // Place a piece for the current player
    int currentPlayerIndex = getCurrentPlayer()->playerNumber();
    Bit *piece = PieceForPlayer(currentPlayerIndex);
    piece->setPosition(holder->getPosition());
    holder->setBit(piece);

    return true;    
}

bool TicTacToe::canBitMoveFrom(Bit *bit, BitHolder *src)
{
    // you can't move anything in tic tac toe
    return false;
}

bool TicTacToe::canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst)
{
    // you can't move anything in tic tac toe
    return false;
}

//
// free all the memory used by the game on the heap
//
void TicTacToe::stopGame()
{
    for (int rowX = 0; rowX < _gameOptions.rowX; rowX++) 
    {
        for (int rowY = 0; rowY < _gameOptions.rowY; rowY++) 
        {
            _grid[rowX][rowY].destroyBit();
        }
    }
    gameOver = false; // Reset so we can play a new game
}

//
// helper function for the winner check
//
Player* TicTacToe::ownerAt(int index) const
{
    Bit *bit = _grid[index % 3][index / 3].bit();
    if (!bit) return nullptr;
    else return bit->getOwner();
}

Player* TicTacToe::checkForWinner()
{
    // The winning combinations to check
    int triples[8][3] = { {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6} };

    // Loop through, checking every combination
    for (int i = 0; i < 8; i++) {
        Player *owner1 = ownerAt(triples[i][0]);
        Player *owner2 = ownerAt(triples[i][1]);
        Player *owner3 = ownerAt(triples[i][2]);
        if ((owner1 && owner2 && owner3) && (owner1 == owner2 && owner2 == owner3)) {
            logger.Event("Player " + std::to_string(owner1->playerNumber()) + " won the game");
            gameOver = true;
            return owner1;
        }
    }
    
    return nullptr;
}

bool TicTacToe::checkForDraw()
{
    for (int rowX = 0; rowX < _gameOptions.rowX; rowX++) 
    {
        for (int rowY = 0; rowY < _gameOptions.rowY; rowY++) 
        {
            // If any space is open on the board, there is no draw
            if (!_grid[rowX][rowY].bit()) return false;
        }
    }
    logger.Event("The game ended in a draw");
    return true;
}

//
// state strings
//
std::string TicTacToe::initialStateString()
{
    return "000000000";
}

//
// this still needs to be tied into imguis init and shutdown
// we will read the state string and store it in each turn object
//
std::string TicTacToe::stateString() const
{
    // return a string representing the current state of the board
    // the string should be 9 characters long, one for each square
    // each character should be '0' for empty, '1' for player 1 (X), and '2' for player 2 (O)
    // the order should be left-to-right, top-to-bottom
    // for example, the starting state is "000000000"
    char gameState[] = "000000000";
    int stateIndex = 0;
    
    // if player 1 has placed an X in the top-left and player 2 an O in the center, the state would be "100020000"
    // you can build the string using a loop and the to_string function
    for (int rowX = 0; rowX < _gameOptions.rowX; rowX++) 
    {
        for (int rowY = 0; rowY < _gameOptions.rowY; rowY++) 
        {
            // for example, to convert an integer to a string, you can use std::to_string(1) which returns "1"
            // you can get the bit at each square using _grid[y][x].bit()
            Bit *bit = _grid[rowX][rowY].bit();
            // if the bit is not null, you can get its owner using bit->getOwner()->playerNumber()
            // remember that player numbers are zero-based, so add 1 to get '1' or '2'
            if (bit) gameState[stateIndex] = '1' + bit->getOwner()->playerNumber(); // Need to add '1' to convert the player number to the correct character
            // if the bit is null, add '0' to the string (do nothing, in my implementation)
            stateIndex++;
        }
    }
    
    // finally, return the constructed string
    return gameState;
}

//
// this still needs to be tied into imguis init and shutdown
// when the program starts it will load the current game from the imgui ini file and set the game state to the last saved state
//
void TicTacToe::setStateString(const std::string &s)
{
    // set the state of the board from the given string
    // the string will be 9 characters long, one for each square
    // each character will be '0' for empty, '1' for player 1 (X), and '2' for player 2 (O)
    // the order will be left-to-right, top-to-bottom
    // for example, the starting state is "000000000"
    // if player 1 has placed an X in the top-left and player 2 an O in the center, the state would be "100020000"
    // you can loop through the string and set each square in _grid accordingly
    // for example, if s[0] is '1', you would set _grid[0][0] to have player 1's piece
    // if s[4] is '2', you would set _grid[1][1] to have player 2's piece
    // if s[8] is '0', you would set _grid[2][2] to be empty
    // you can use the PieceForPlayer function to create a new piece for a player
    // remember to convert the character to an integer by subtracting '0'
    // for example, int playerNumber = s[index] - '0';
    // if playerNumber is 0, set the square to empty (nullptr)
    // if playerNumber is 1 or 2, create a piece for that player and set it in the square
    // finally, make sure to position the piece at the holder's position
    // you can get the position of a holder using holder->getPosition()
    // loop through the 3x3 array and set each square accordingly
    // the string should always be valid, so you don't need to check its length or contents
    // but you can assume it will always be 9 characters long and only contain '0', '1', or '2'
}


//
// this is the function that will be called by the AI
//
void TicTacToe::updateAI() 
{
    // we will implement the AI in the next assignment!
}

