//
//  MinimaxWrapper.c
//  UltimateTicTacToe
//
//  Created by Mason Raffo on 4/15/21.
//

#include "MinimaxWrapper.hpp"

#include "Minimax.hpp"

CGameState * CreateCppClass(){
    return new GameState();
}

void ReleaseCppClass(CGameState * c){
    delete c;
}

int GetPosition(CGameState * c, int board, int piece){
    return c->getPosition(board, piece);
}

void SetPosition(CGameState * c, int boardLocation, int pieceLocation, int piece) {
    c->setPosition(boardLocation, pieceLocation, piece);
}

void Move(CGameState * c, int boardLocation, int pieceLocation) {
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
