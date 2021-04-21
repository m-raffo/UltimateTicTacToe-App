#pragma once

#include <bitset>
#include <vector>
#include <iostream>


struct nnInput {
    float board[199] = {};
    float valid[81] = {};
};

struct nnOutput {
    float policy[81] = {};
    float value = 0;
};

const std::bitset<20> winningPosX[] = {
    0b00000000000001010100,
    0b00000001010100000000,
    0b01010100000000000000,
    0b00000100000100000100,
    0b00010000010000010000,
    0b01000001000001000000,
    0b01000000010000000100,
    0b00000100010001000000,
};

const std::bitset<20> winningPosO[] = {
    0b00000000000010101000,
    0b00000010101000000000,
    0b10101000000000000000,
    0b00001000001000001000,
    0b00100000100000100000,
    0b10000010000010000000,
    0b10000000100000001000,
    0b00001000100010000000,
};

struct boardCoords {
    int board, piece;
};

/**
 * Evaluates the give miniboard to check for a win, including the possibility that a square is tied.
 * This is necessary for checking for wins on the larger, overall board.
 * 0: Ongoing game
 * 1: X win
 * 2: O win
 * 3: Tie
 */
int checkMiniboardResultsWithTie(std::bitset<20> miniboard);

/**
 * Evaluates the given miniboard to check for wins.
 * 0: Ongoing game
 * 1: X win
 * 2: O win
 * 3: Tie
 */
int checkMiniboardResults(std::bitset<20> miniboard);

/**
 * Gets result if miniboard has been marked with final result. Will not check for wins for the benefit of computation speed.
 * 0: Ongoing game
 * 1: X wins
 * 2: O wins
 * 3: Tie
 */
int getMiniboardResults(std::bitset<20> miniboard);


class GameState {
    private:
        /**
         * Info - stores information about the GameState that is not stored on the board
         * Bits:
         * 0-3: Required board to move on
         * 4: Is there a required board (1=yes, 0=no)
         * 5: Player to move (1=X, 0=O)
         */ 

    public:
    int info;
    boardCoords previousMove;

    bool isValidMove(int board, int piece);

    std::bitset<20> board[9];

    GameState();

    void setToMove(int m);
    int getToMove();

    void setRequiredBoard(int requiredBoard);
    int getRequiredBoard();

    /**
     * Sets the specificed location in the board to the given piece.
     *
     * @param boardLocation The board (from 0 to 8) to get
     * @param pieceLocation The piece (fromr 0 to 8) to get
     * @param piece The piece to set. 1 for X; 2 for O; 0 for empty
     */
    void setPosition(int boardLocation, int pieceLocation, int piece);
    
    /**
     * Gets the piece in the specified location in the board.
     *
     * @param boardLocation The board (from 0 to 8) to get
     * @param pieceLocation The piece (fromr 0 to 8) to get
     * @return 0 if the position is empty, 1 if the position is claimed by X, and 2 if the position is claimed by O
     */
    int getPosition(int boardLocation, int pieceLocation);

    /**
     * Performs the specificed move on the board, moving the piece whose turn it is, and flipping toMove.
     */
    void move(int boardLocation, int pieceLocation);

    /**
     * Updates the game statuses of all the miniboards, checking to see if any of them are won. If a position is won for both O and X and not already marked, it will be marked as a win for X.
     */
    void updateMiniboardStatus();

    /**
     * Updates the status of a single miniboard to see if it is claimed.
     */
    void updateSignleMiniboardStatus(int boardIndex);

    /**
     * Gets the status of the given miniboard.
     * Important: GameState.updateMiniboardStatus() MUST be called before this function to ensure correct results.
     *
     * 0: Ongoing game
     * 1: X wins
     * 2: O wins
     * 3: Tie
     * @param boardLocation the board to check from 0 to 8
     * @return the status
     */
    int getBoardStatus(int boardLocation);

    /**
     * Gets the status of the entire game.
     * Important: GameState.updateMiniboardStatus() MUST be called before this function to ensure correct results.
     *
     * 0: Ongoing game
     * 1: X wins
     * 2: O wins
     * 3: Tie
     * @return the status
     */
    int getStatus();

    /**
     * Gets a copy of the board
     */
    GameState getCopy();

    std::vector<GameState> allPossibleMoves();

    /**
     * Gets the board and piece of an absolute index of the full size 9x9 board.
     */
    boardCoords absoluteIndexToBoardAndPiece(int i);

    /**
     * Converts the current board to a human-readable string representation and writes it to cout.
     */
    void displayGame();
    
    std::string gameToString();

    /**
     * Gets the current board in the Canonical form for
     * input to the NN.
     *
     * The board is stored in the form 9 boards of 22 bits.
     *
     * Bits 0-17 store the state of each of the 9 spots. If
     * the first bit is set, the player to move has this
     * spot, if the second bit is set, the opposing player
     * has the spot.
     *
     * Bit 18 is set if the player to move has won the board
     * Bit 19 is set if the opposing player has won the board
     * Bit 20 is set if the board is tied
     *
     * Bit 21 is set if the player is allowed to move on the
     * board
     *
     * Bit 199 is unused; it is leftover from saving toMove
     * on a full board.
     *
     * Board is then converted to a vector<int> for output
     */
    std::vector<int> getCanonicalBoard();
    
    /**
     * Gets a representation of the board as a single
     * bitset object.
     *
     * The board is stored in the form 9 boards of 22 bits.
     *
     * Bits 0-17 store the state of each of the 9 spots. If
     * the first bit is set, player 1 has this
     * spot, if the second bit is set, the player 2 has the
     * spot.
     *
     * Bit 18 is set if player 1 has won the board
     * Bit 19 is set if player 2 has won the board
     * Bit 20 is set if the board is tied
     *
     * Bit 21 is set if the player is allowed to move on the
     * board
     *
     * Bit 198 of the bitset is set if player 1 (X) is to
     * move and unset if player 2 (O) is to move
     */
    std::vector<int> getBoardBitset();

    std::vector<int> getAllPossibleMovesVector();

    /**
     * Gets the current board in the Canonical form for
     * input to the NN.
     *
     * The board is stored in the form 9 boards of 22 bits.
     *
     * Bits 0-17 store the state of each of the 9 spots. If
     * the first bit is set, the player to move has this
     * spot, if the second bit is set, the opposing player
     * has the spot.
     *
     * Bit 18 is set if the player to move has won the board
     * Bit 19 is set if the opposing player has won the board
     * Bit 20 is set if the board is tied
     *
     * Bit 21 is set if the player is allowed to move on the
     * board
     *
     * Bit 199 is unused; it is leftover from saving toMove
     * on a full board.
     *
     * If a player has won a miniboard, all of the bits
     * corresponding to spots on that miniboard will be set
     * for that player, and none of the spots for the
     * opponent.
     *
     * Board is then converted to a vector<int> for output
     */
    std::bitset<199> getCanonicalBoardBitset();

    nnInput getNNInput();
    
};

GameState boardVector2GameState(std::vector<int> board);
