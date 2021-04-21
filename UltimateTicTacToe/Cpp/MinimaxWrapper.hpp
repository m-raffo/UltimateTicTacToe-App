//
//  MinimaxWrapper.h
//  UltimateTicTacToe
//
//  Created by Mason Raffo on 4/15/21.
//

#ifndef MinimaxWrapper_h
#define MinimaxWrapper_h

typedef struct GameState CGameState;
typedef struct CboardCoords CboardCoords;
typedef struct MCTS CMCTS;
typedef struct CnnInput CnnInput;
typedef struct CnnOutput CnnOutput;

struct CboardCoords {
    int board, piece;
};

struct CnnInput {
    float board[199];
    float valid[81];
};

struct CnnOutput {
    float policy[81];
    float value;
};

#ifdef __cplusplus
extern "C" {
#endif
CGameState * CreateGameState();

/**
 Creates a new Monte Carlo Search Tree object.
 @return A pointer to the new object
 */
CMCTS * CreateMCTS(int cpuct, double dirichlet);

/**
 Starts a new search from the given position.
 */
void StartNewSearch(CMCTS * mcts, CGameState * position);

/**
 Perform a the first half of a Monte Carlo iteration. A neural network evaluation must be performed on the returned values and then SearchPostNN() called with the results.
 */
CnnInput SearchPreNN(CMCTS * mcts);

/**
 Finish the search after the neural network evaluation has been completed.
 */
void SearchPostNN(CMCTS * mcts, CnnOutput);

/**
 Gets the current best action in the Monte Carlo tree.
 @return The action (ranging from 0..<81)
 */
int GetBestAction(CMCTS * mcts);

/**
 Performs the given action on the tree.
 @param action The action from 0..<81
 */
void TakeActionMCTS(CMCTS * mcts, int action);

/**
 Checks if a neural network evaluation is needed after the last SearchPreNN.
 @return 1 if evaluation is needed; 0 if evaluation is not needed
 */
int IsEvalNeeded(CMCTS * mcts);

int GetBoardStatus(CGameState * c, int board);

void ReleaseGameState(CGameState * c);
void ReleaseMCTS(CMCTS * mcts);

int GetPosition(CGameState * c, int board, int piece);

void SetPosition(CGameState * c, int boardLocation, int pieceLocation, int piece);

void GameStateMove(CGameState * c, int boardLocation, int pieceLocation);

int IsValidMove(CGameState * c, int board, int piece);

int GetRequiredBoard(CGameState * c);

int GetToMove(CGameState * c);

CboardCoords MinimaxSearchMove(CGameState * position, int depth, int playAsX);

CboardCoords MinimaxSearchTimeMove(CGameState * position, int time, int playAsX);

    
#ifdef __cplusplus
}
#endif

#endif /* MinimaxWrapper_h */
