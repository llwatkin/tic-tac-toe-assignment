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

const int AI_PLAYER    = 1;      // index of the AI player (O)
const int HUMAN_PLAYER = 0;      // index of the human player (X)

Logger &logger = Logger::GetInstance();

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
    setAIPlayer(AI_PLAYER);
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
    if (_gameOptions.gameOver) return false;
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
    _gameOptions.gameOver = false; // Reset so we can play a new game
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
            _gameOptions.gameOver = true;
            return owner1;
        }
    }
    
    return nullptr;
}

//
// A different winner checking function that using a string game state, rather than the current board (used for AI)
//
Player* TicTacToe::checkForWinnerWithGameState(std::string gameState) 
{
    // The winning combinations to check
    int triples[8][3] = { {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6} };

    // Loop through, checking every combination
    for (int i = 0; i < 8; i++)
    {
        if (gameState[triples[i][0]] != '0' && gameState[triples[i][1]] != '0' && gameState[triples[i][2]] != '0') 
        {
            if (gameState[triples[i][0]] == gameState[triples[i][1]] && gameState[triples[i][1]] == gameState[triples[i][2]])
            {
                int playerNumber = gameState[triples[i][0]] == '1' ? 0 : 1;
                return getPlayerAt(playerNumber);
            }
        }
    }

    return nullptr;
}

bool TicTacToe::checkForDraw()
{
    if (_gameOptions.gameOver) return false;

    for (int rowX = 0; rowX < _gameOptions.rowX; rowX++) 
    {
        for (int rowY = 0; rowY < _gameOptions.rowY; rowY++) 
        {
            // If any space is open on the board, there is no draw
            if (!_grid[rowX][rowY].bit()) return false;
        }
    }

    logger.Event("The game ended in a draw");
    _gameOptions.gameOver = true;
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
    char gameState[] = "000000000";
    int stateIndex = 0;
    
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
    //int stateIndex = 0;
    //for (int rowX = 0; rowX < _gameOptions.rowX; rowX++) 
    //{
    //    for (int rowY = 0; rowY < _gameOptions.rowY; rowY++) 
    //    {
            // for example, if s[0] is '1', you would set _grid[0][0] to have player 1's piece
            // if s[4] is '2', you would set _grid[1][1] to have player 2's piece
            // if s[8] is '0', you would set _grid[2][2] to be empty
            // you can use the PieceForPlayer function to create a new piece for a player
            // remember to convert the character to an integer by subtracting '0'
            //int playerNumber = s[stateIndex] - '0';
            // if playerNumber is 0, set the square to empty (nullptr)
            // if playerNumber is 1 or 2, create a piece for that player and set it in the square
            //if (playerNumber == 0) _grid[rowX][rowY] = nullptr;
            //else _grid[rowX][rowY] = PieceForPlayer(s[stateIndex] - '0');
            //stateIndex++;

            // TODO
            // finally, make sure to position the piece at the holder's position
            // you can get the position of a holder using holder->getPosition()
            // loop through the 3x3 array and set each square accordingly
            
    //    }
    //}
    // the string should always be valid, so you don't need to check its length or contents
    // but you can assume it will always be 9 characters long and only contain '0', '1', or '2'
}

//
// Find all possible moves for the given player
//
std::vector<std::string> TicTacToe::generateMoves(std::string gameState, int playerNumber) 
{
    // Return state strings for every possible move from the current player's perspective
    std::vector<std::string> moves;

    for (size_t i = 0; i < gameState.length(); i++)
    {
        if (gameState[i] == '0')
        {
            std::string move = gameState;
            move[i] = '1' + playerNumber;
            moves.push_back(move);
        }
    }
    
    return moves;
}

//
// If there's a winner, return 1 if the current player has won, -1 if the opponent won, and 0 if its a draw
//
int TicTacToe::evaluate(std::string gameState, int playerNumber) 
{
    Player *winner = checkForWinnerWithGameState(gameState);
    if (winner)
    {
        int winnerNumber = winner->playerNumber();
        if (winnerNumber == playerNumber) return 1;
        else return -1;
    }
    return 0;
}

//
// Find the most optimal move by evaluating all possible games stemming from that move
//
int TicTacToe::negamax(std::string gameState, int depth, int playerNumber)
{
    if (depth == 0 || checkForWinnerWithGameState(gameState)) return evaluate(gameState, playerNumber);
    std::vector<std::string> moves = generateMoves(gameState, playerNumber);
    if (moves.empty()) return 0;

    int value = -2;
    int nextPlayer = playerNumber == 0 ? 1 : 0;
    for (auto const & move : moves) value = std::max(value, -negamax(move, depth - 1, nextPlayer));
    return value;
}

//
// Negamax wrapper function to get the best move for the AI player
//
std::string TicTacToe::getBestMove() 
{
    std::string gameState = stateString();
    std::vector<std::string> moves = generateMoves(gameState, AI_PLAYER);
    std::string bestMove = "000000000";
    int bestEvaluation = -2;

    for (auto const & move : moves) 
    {
        int evaluation = -negamax(move, 3, HUMAN_PLAYER);
        logger.Info("Checking move: " + move + " Evaluation: " + std::to_string(evaluation));
        if (evaluation > bestEvaluation) 
        {
            bestMove = move;
            bestEvaluation = evaluation;
            logger.Event("Chose a new best move: " + bestMove + " Evaluation: " + std::to_string(bestEvaluation));
        }
    }

    return bestMove;
}


//
// this is the function that will be called by the AI
//
void TicTacToe::updateAI() 
{
    if (_gameOptions.gameOver) return;
    if (_gameOptions.AIPlaying) return;
    else
    {
        _gameOptions.AIPlaying = true;

        std::string bestMove = getBestMove();
        logger.Info("Best AI move: " + bestMove);

        std::string gameState = stateString();
        int stateIndex = 0;
        for (int rowX = 0; rowX < _gameOptions.rowX; rowX++)
        {
            for (int rowY = 0; rowY < _gameOptions.rowY; rowY++)
            {
                // To find the holder for the new move, loop through the board until the place
                // where the gameState and the bestMove differ
                if (gameState[stateIndex] != bestMove[stateIndex])
                {
                    Square *holder = &_grid[rowX][rowY];
                    if (actionForEmptyHolder(holder)) 
                    {
                        _gameOptions.AIPlaying = false;
                        endTurn();
                        logger.Event("AI placed a piece at (" + std::to_string(rowX) + ", " + std::to_string(rowY) + ")");

                    }
                    else
                    {
                        logger.Error("updateAI(): Failed to place piece at (" + std::to_string(rowX) + ", " + std::to_string(rowY) + ")");
                    }
                    
                    return;
                }
                stateIndex++;
            }
        }
    }
}
