#pragma once
#include "Game.h"
#include "Square.h"
#include <algorithm>
#include <vector>

//
// the classic game of tic tac toe
//

//
// the main game class
//
class TicTacToe : public Game
{
public:
    TicTacToe();
    ~TicTacToe();

    // set up the board
    void        setUpBoard() override;

    Player*     checkForWinner() override;
    Player*     checkForWinnerWithGameState(std::string gameState);
    bool        checkForDraw() override;
    std::string initialStateString() override;
    std::string stateString() const override;
    void        setStateString(const std::string &s) override;
    bool        actionForEmptyHolder(BitHolder *holder) override;
    bool        canBitMoveFrom(Bit*bit, BitHolder *src) override;
    bool        canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst) override;
    void        stopGame() override;

    std::vector<std::string> generateMoves(std::string gameState, int playerNumber);
    int         evaluate(std::string gameState, int playerNumber);
    int         negamax(std::string gameState, int depth, int playerNumber);
    std::string getBestMove();
	void        updateAI() override;
    bool        gameHasAI() override { return true; }
    BitHolder &getHolderAt(const int x, const int y) override { return _grid[y][x]; }
private:
    Bit *       PieceForPlayer(const int playerNumber);
    Player*     ownerAt(int index ) const;

    Square      _grid[3][3];
};

