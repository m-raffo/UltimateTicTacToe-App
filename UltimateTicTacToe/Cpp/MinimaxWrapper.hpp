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

struct CboardCoords {
    int board, piece;
};

#ifdef __cplusplus
extern "C" {
#endif
CGameState * CreateCppClass();

void ReleaseCppClass(CGameState * c);

int GetPosition(CGameState * c, int board, int piece);

void SetPosition(CGameState * c, int boardLocation, int pieceLocation, int piece);

void Move(CGameState * c, int boardLocation, int pieceLocation);

int IsValidMove(CGameState * c, int board, int piece);

CboardCoords MinimaxSearchMove(CGameState * position, int depth, int playAsX);

CboardCoords MinimaxSearchTimeMove(CGameState * position, int time, int playAsX);

    
#ifdef __cplusplus
}
#endif

#endif /* MinimaxWrapper_h */
