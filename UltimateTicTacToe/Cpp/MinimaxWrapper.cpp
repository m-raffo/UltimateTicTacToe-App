//
//  MinimaxWrapper.c
//  UltimateTicTacToe
//
//  Created by Mason Raffo on 4/15/21.
//

#include "MinimaxWrapper.hpp"

#include "Minimax.hpp"
#include "MonteCarlo.hpp"

CGameState * CreateGameState(){
    return new GameState();
}

CMCTS * CreateMCTS(int cpuct, double dirichlet) {
    return new MCTS(cpuct, dirichlet, 0);
};

void StartNewSearch(CMCTS * mcts, CGameState * position) {
    mcts->startNewSearch(*position);
}


CnnInput SearchPreNN(CMCTS * mcts) {
    CnnInput result;
    nnInput resultCpp = mcts->searchPreNNforTfLite();
    
    memcpy(result.board, resultCpp.board, sizeof resultCpp.board);
    memcpy(result.valid, resultCpp.valid, sizeof resultCpp.valid);
    
    return result;
}

void SearchPostNN(CMCTS * mcts, CnnOutput nnResult) {
    nnOutput nnCpp;
    
    memcpy(nnCpp.policy, nnResult.policy, sizeof nnResult.policy);
    nnCpp.value = nnResult.value;
    
    mcts->searchPostNNTfLite(nnCpp);
}

int GetBestAction(CMCTS * mcts) {
    return mcts->maxActionProb();
}

void TakeActionMCTS(CMCTS * mcts, int action) {
    mcts->takeAction(action);
}

void ReleaseGameState(CGameState * c){
    delete c;
}

void ReleaseMCTS(CMCTS * mcts) {
    delete mcts;
}

int IsEvalNeeded(CMCTS * mcts) {
    return (int)mcts->evaluationNeeded;
}

int GetPosition(CGameState * c, int board, int piece){
    return c->getPosition(board, piece);
}

void SetPosition(CGameState * c, int boardLocation, int pieceLocation, int piece) {
    c->setPosition(boardLocation, pieceLocation, piece);
}

void GameStateMove(CGameState * c, int boardLocation, int pieceLocation) {
    c->move(boardLocation, pieceLocation);
}

int IsValidMove(CGameState * c, int board, int piece) {
    return c->isValidMove(board, piece);
}

int GetRequiredBoard(CGameState * c) {
    return c->getRequiredBoard();
}

CboardCoords MinimaxSearchMove(CGameState * position, int depth, int playAsX) {
    CboardCoords result;
    
    boardCoords firstResult = minimaxSearchMove(*position, depth, playAsX == 1);
    
    result.board = firstResult.board;
    result.piece = firstResult.piece;
    
    return result;
}

CboardCoords MinimaxSearchTimeMove(CGameState * position, int time, int playAsX) {
    CboardCoords result;
    
    boardCoords firstResult = minimaxSearchTimeMove(*position, time, playAsX == 1);
    
    result.board = firstResult.board;
    result.piece = firstResult.piece;
    
    return result;

}
