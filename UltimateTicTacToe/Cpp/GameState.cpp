#include "GameState.hpp"

#include <bitset>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Masks for winning positions
// const bitset<20> winningPosX[] = {
//     0b00000000000001010100,
//     0b00000001010100000000,
//     0b01010100000000000000,
//     0b00000100000100000100,
//     0b00010000010000010000,
//     0b01000001000001000000,
//     0b01000000010000000100,
//     0b00000100010001000000,
// };

// const bitset<20> winningPosO[] = {
//     0b00000000000010101000,
//     0b00000010101000000000,
//     0b10101000000000000000,
//     0b00001000001000001000,
//     0b00100000100000100000,
//     0b10000010000010000000,
//     0b10000000100000001000,
//     0b00001000100010000000,
// };

int checkMiniboardResultsWithTie(bitset<20> miniboard) {
  bitset<20> posToCheck;

  // If the position is empty, return 0
  if (miniboard == posToCheck) {
    return 0;
  }

  bool emptySpace = false;

  // Check each winning possibility
  for (int j = 0; j <= 7; j++) {
    // Match the position with the winning mask
    posToCheck = winningPosX[j];
    posToCheck &= miniboard;

    // If the position matches the winning mask, check that none of the squares
    // are claimed by O
    if (posToCheck == winningPosX[j]) {
      // Check that the winningPosO mask is all negative on the miniboard
      posToCheck = winningPosO[j];
      posToCheck &= ~(miniboard);

      // If O has no claims to squares that X used to win, it is a won position
      if (posToCheck == winningPosO[j]) {
        return 1;
      }
    }

    // Same for O
    posToCheck = winningPosO[j];
    posToCheck &= miniboard;

    if (posToCheck == winningPosO[j]) {
      // Check that the winningPosX mask is all negative on the miniboard
      posToCheck = winningPosX[j];
      posToCheck &= ~(miniboard);

      // If X has no claims to squares that X used to win, it is a won position
      if (posToCheck == winningPosX[j]) {
        return 2;
      }
    }
  }

  // Check if the position is completely filled
  for (int j = 0; j <= 8; j++) {
    int location = 2 + (j * 2);
    if (!miniboard[location] && !miniboard[location + 1]) {
      emptySpace = true;
      break;
    }
  }

  // If there are no empty spaces, mark the position as a tie
  if (!emptySpace) {
    return 3;
  }

  // No wins found and not a tie
  return 0;
}

int checkMiniboardResults(bitset<20> miniboard) {
  bitset<20> posToCheck;

  // If the position is empty, return 0
  if (miniboard == posToCheck) {
    return 0;
  }

  bool emptySpace = false;

  // Check each winning possibility
  for (int j = 0; j <= 7; j++) {
    // Match the position with the winning mask
    posToCheck = winningPosX[j];
    posToCheck &= miniboard;

    // If the position matches the winning mask, mark it as a win
    if (posToCheck == winningPosX[j]) {
      return 1;
    }

    // Same for O
    posToCheck = winningPosO[j];
    posToCheck &= miniboard;

    if (posToCheck == winningPosO[j]) {
      return 2;
    }
  }

  // Check if the position is completely filled
  for (int j = 0; j <= 8; j++) {
    int location = 2 + (j * 2);
    if (!miniboard[location] && !miniboard[location + 1]) {
      emptySpace = true;
      break;
    }
  }

  // If there are no empty spaces, mark the position as a tie
  if (!emptySpace) {
    return 3;
  }

  return 0;
}

int getMiniboardResults(bitset<20> miniboard) {
  // Tie
  if (miniboard[0] && miniboard[1]) {
    return 3;
  }

  // X wins
  else if (miniboard[0]) {
    return 1;
  }

  // O wins
  else if (miniboard[1]) {
    return 2;
  }

  // Ongoing game
  else {
    return 0;
  }
}

GameState::GameState() {
  info = 32;  // Default is X to move
  previousMove.board = -1;
  previousMove.piece = -1;
}

void GameState::setToMove(int m) {
  /**
   * Sets the player to move
   * 1:X
   * 2:O
   */
  if (m == 1) {
    // Set bit 5 of info to 1
    info |= 1 << 5;
  } else {
    // Set bit 5 of info to 0
    info &= ~(1 << 5);
  }
}

int GameState::getToMove() {
  /**
   * Sets the player to move
   * 1:X
   * 2:O
   */
  // Check bit 5 of info is set
  char toMove = 1 << 5;
  toMove &= info;
  if (toMove) {
    return 1;
  } else {
    return 2;
  }
}

void GameState::setRequiredBoard(int requiredBoard) {
  /**
   * Sets the required board for the next move. If the board is already claimed,
   * set no required board. 0-8 = that board to move on -1 = no required board
   */

  // If the board is claimed
  if (board[requiredBoard][0] || board[requiredBoard][1]) {
    requiredBoard = -1;
  }

  // set bit 4 to 0 if there is no required board
  if (requiredBoard == -1) {
    info &= ~(1 << 4);
  } else {
    // set bit 4 to 1 b/c there is a required board
    info |= 1 << 4;

    // Set bits 0-3 to 0 while leaving everything else untouched
    info &= 0b111111110000;

    // Set bits 0-3 to the correct required board
    info |= requiredBoard;
  }
}

int GameState::getRequiredBoard() {
  /**
   * Gets the required board for the next move.
   * 0-8: That board must be moved on
   * -1: No required board
   */

  // Check if there is a required board
  char requiredBoard = 1 << 4;
  requiredBoard &= info;

  if (requiredBoard) {
    requiredBoard = 15;  // Sets the first 4 bits to 1

    requiredBoard &= info;
    return requiredBoard;
  } else {
    return -1;
  }
}

int GameState::getPosition(int boardLocation, int pieceLocation) {
  // Miniboards are 20 bits longs
  // Spots are 2 bits long
  // The first two bits of each miniboard are for storing the results of the
  // miniboard to avoid recalculation if possible
  int location = (2 * pieceLocation) + 2;

  if (board[boardLocation][location]) {
    return 1;
  } else if (board[boardLocation][location + 1]) {
    return 2;
  } else {
    return 0;
  }
}

void GameState::setPosition(int boardLocation, int pieceLocation, int piece) {
  int location = (2 * pieceLocation) + 2;

  if (piece == 0) {
    board[boardLocation][location] = 0;
    board[boardLocation][location + 1] = 0;
  } else if (piece == 1) {
    board[boardLocation][location] = 1;
    board[boardLocation][location + 1] = 0;
  } else {
    board[boardLocation][location] = 0;
    board[boardLocation][location + 1] = 1;
  }
}

void GameState::move(int boardLoaction, int pieceLocation) {
  setPosition(boardLoaction, pieceLocation, getToMove());
  updateSignleMiniboardStatus(boardLoaction);
  setRequiredBoard(pieceLocation);

  // Flip the player to mvoe
  if (getToMove() == 1) {
    setToMove(2);
  } else {
    setToMove(1);
  }

  previousMove.board = boardLoaction;
  previousMove.piece = pieceLocation;
}

void GameState::updateMiniboardStatus() {
  // Loop through each miniboard
  for (int i = 0; i <= 8; i++) {
    // Check if already marked as a finished position
    updateSignleMiniboardStatus(i);
  }
}

void GameState::updateSignleMiniboardStatus(int boardIndex) {
  // Check if already marked as a finished position
  if (board[boardIndex][0] || board[boardIndex][1]) {
    return;
  }

  int result = checkMiniboardResults(board[boardIndex]);

  // Ongoing game
  if (!result) {
    return;
  }

  // X Wins
  else if (result == 1) {
    board[boardIndex][0] = 1;
  }

  // O Wins
  else if (result == 2) {
    board[boardIndex][1] = 1;
  }

  // Tie
  else {
    board[boardIndex][0] = 1;
    board[boardIndex][1] = 1;
  }
}

int GameState::getBoardStatus(int boardLocation) {
  // Tie
  if (board[boardLocation][0] && board[boardLocation][1]) {
    return 3;
  }

  // X wins
  else if (board[boardLocation][0]) {
    return 1;
  }

  // O wins
  else if (board[boardLocation][1]) {
    return 2;
  }

  // Ongoing game
  else {
    return 0;
  }
}

int GameState::getStatus() {
  bitset<20> boardResults;

  // TODO: Assign two bits in GameState.info to track if the game has been
  // evaluated to a winning position

  for (int i = 0; i <= 8; i++) {
    // The location in board results to store this result
    int location = 2 + (i * 2);
    int result = getBoardStatus(i);

    switch (result) {
      // X win
      case 1:
        boardResults[location] = 1;
        break;

      // O win
      case 2:
        boardResults[location + 1] = 1;
        break;

      // Tie
      case 3:
        boardResults[location] = 1;
        boardResults[location + 1] = 1;
        break;

      // Ongoing game or tie - neither person wins the board
      default:
        break;
    }
  }

  return checkMiniboardResultsWithTie(boardResults);
}

GameState GameState::getCopy() {
  GameState copyBoard;

  // Copy the board
  for (int i = 0; i < 9; i++) {
    copyBoard.board[i] = board[i];
  }
  copyBoard.info = info;

  return copyBoard;
}

bool GameState::isValidMove(int board, int piece) {
  if (getBoardStatus(board) != 0) {
    return false;
  }
  if (getRequiredBoard() != -1) {
    if (board == getRequiredBoard() && getPosition(board, piece) == 0) {
      return true;
    }
  } else {
    if (getPosition(board, piece) == 0) {
      return true;
    }
  }

  return false;
}

vector<GameState> GameState::allPossibleMoves() {
  vector<GameState> allMoves;

  int requiredBoard = getRequiredBoard();

  // If there is a required board
  if (requiredBoard > -1) {
    // Check every spot on the required board
    for (int i = 0; i < 9; i++) {
      int location = 2 + (i * 2);

      // If the spot is empty, add this as a move
      if (!board[requiredBoard][location] &&
          !board[requiredBoard][location + 1]) {
        GameState newBoard = getCopy();

        newBoard.move(requiredBoard, i);

        allMoves.push_back(newBoard);
      }
    }
  } else {
    // Loop every board
    for (int boardIndex = 0; boardIndex < 9; boardIndex++) {
      // If the game is over in this board, no moves are possible on it
      if (getBoardStatus(boardIndex)) continue;

      for (int i = 0; i < 9; i++) {
        int location = 2 + (i * 2);

        // If the spot are empty, add this as a move
        if (!board[boardIndex][location] && !board[boardIndex][location + 1]) {
          GameState newBoard = getCopy();

          newBoard.move(boardIndex, i);

          allMoves.push_back(newBoard);
        }
      }
    }
  }

  return allMoves;
}

vector<int> GameState::getAllPossibleMovesVector() {
  vector<int> moves(81);

  int requiredBoard = getRequiredBoard();

  // If there is a required board
  if (requiredBoard > -1) {
    // Check every spot on the required board
    for (int i = 0; i < 9; i++) {
      int location = 2 + (i * 2);

      // If the spot is empty, add this as a move
      if (!board[requiredBoard][location] &&
          !board[requiredBoard][location + 1]) {
        moves[requiredBoard * 9 + i] = 1;
      }
    }
  } else {
    // Loop every board
    for (int boardIndex = 0; boardIndex < 9; boardIndex++) {
      // If the game is over in this board, no moves are possible on it
      if (getBoardStatus(boardIndex)) continue;

      for (int i = 0; i < 9; i++) {
        int location = 2 + (i * 2);

        // If the spot are empty, add this as a move
        if (!board[boardIndex][location] && !board[boardIndex][location + 1]) {
          moves[boardIndex * 9 + i] = 1;
        }
      }
    }
  }

  return moves;
}

boardCoords GameState::absoluteIndexToBoardAndPiece(int i) {
  /*
  i = index
  gx = global x value (independent of board)
  gy = same

  lx = local x value (within board)
  ly = same

  bx = x value of the whole board
  by = same

  pi = piece index
  bi = board index
  */
  int gx, gy, lx, ly, bx, by, pi, bi;
  boardCoords result;

  gx = i % 9;
  gy = i / 9;

  lx = gx % 3;
  ly = gy % 3;

  bx = (i % 9) / 3;
  by = i / 27;

  pi = ly * 3 + lx;
  bi = by * 3 + bx;

  result.board = bi;
  result.piece = pi;

  return result;
}

string GameState::gameToString() {
  string output = "";

  if (getToMove() == 1) {
    output += "X to move\n";
  } else {
    output += "O to move\n";
  }

  int requiredBoard = getRequiredBoard();
  if (requiredBoard != -1) {
    output += "Required board: " + to_string(requiredBoard) + '\n';
  } else {
    output += "Required board: None\n";
  }

  int absolutePieceIndex, location;
  boardCoords coords;

  for (int row = 0; row < 9; row++) {
    for (int boardRow = 0; boardRow < 3; boardRow++) {
      for (int col = 0; col < 3; col++) {
        absolutePieceIndex = (row * 9) + (boardRow * 3) + col;
        coords = absoluteIndexToBoardAndPiece(absolutePieceIndex);
        location = 2 + (2 * coords.piece);

        if (board[coords.board][location]) {
          output += "\033[31mX\033[0m";
        } else if (board[coords.board][location + 1]) {
          output += "\033[94mO\033[0m";
        } else {
          output += " ";
        }

        // Give a divider if not on the last one
        if (col != 2) {
          output += " | ";
        }
      }

      output += "\\\\ ";
    }

    if ((row + 1) % 3 != 0) {
      output += "\n---------   ---------   ---------   \n";
    } else {
      output += "\n=================================\n";
    }
  }

  return output;
}

void GameState::displayGame() {
  if (getToMove() == 1) {
    cout << "X to move\n";
  } else {
    cout << "O to move\n";
  }

  int requiredBoard = getRequiredBoard();
  if (requiredBoard != -1) {
    cout << "Required board: " << requiredBoard << '\n';
  } else {
    cout << "Required board: None\n";
  }

  int absolutePieceIndex, location;
  boardCoords coords;

  for (int row = 0; row < 9; row++) {
    for (int boardRow = 0; boardRow < 3; boardRow++) {
      for (int col = 0; col < 3; col++) {
        absolutePieceIndex = (row * 9) + (boardRow * 3) + col;
        coords = absoluteIndexToBoardAndPiece(absolutePieceIndex);
        location = 2 + (2 * coords.piece);

        if (board[coords.board][location]) {
          cout << "\033[31mX\033[0m";
        } else if (board[coords.board][location + 1]) {
          cout << "\033[94mO\033[0m";
        } else {
          cout << " ";
        }

        // Give a divider if not on the last one
        if (col != 2) {
          cout << " | ";
        }
      }

      cout << "\\\\ ";
    }

    if ((row + 1) % 3 != 0) {
      cout << "\n---------   ---------   ---------   \n";
    } else {
      cout << "\n=================================\n";
    }
  }
}

std::vector<int> GameState::getBoardBitset() {
  bitset<199> result;

  int toMove = getToMove();
  int requiredBoard = getRequiredBoard();

  for (int miniboardIndex = 0; miniboardIndex < 9; miniboardIndex++) {
    for (int spotIndex = 0; spotIndex < 9; spotIndex++) {
      int spotStatus = getPosition(miniboardIndex, spotIndex);

      if (spotStatus == 1) {
        result[miniboardIndex * 22 + spotIndex * 2] = 1;
      }

      // If the spot belongs to the other player
      else if (spotStatus == 2) {
        result[miniboardIndex * 22 + spotIndex * 2 + 1] = 1;
      }

      // Else neither owns it, zero is default
    }

    int boardStatus = getBoardStatus(miniboardIndex);

    // Mark if the board is won/lost/tied
    if (boardStatus == 1) {
      result[miniboardIndex * 22 + 18] = 1;
    }

    else if (boardStatus == 2) {
      result[miniboardIndex * 22 + 19] = 1;
    }

    else if (boardStatus == 3) {
      result[miniboardIndex * 22 + 20] = 1;
    }

    // Mark if this board is legal to move in
    // Only legal if unclaimed
    if (boardStatus == 0 &&
        (requiredBoard == miniboardIndex || requiredBoard == -1)) {
      result[miniboardIndex * 22 + 21] = 1;
    }
  }

  // Set the player to move
  if (toMove == 1) {
    result[199] = 1;
  }

  vector<int> fresult;

  for (int i = 0; i < 198; i++) {
    fresult.push_back(result[i]);
  }

  return fresult;
};

vector<int> GameState::getCanonicalBoard() {
  bitset<199> canonical;

  int toMove = getToMove();
  int requiredBoard = getRequiredBoard();

  for (int miniboardIndex = 0; miniboardIndex < 9; miniboardIndex++) {
    int boardStatus = getBoardStatus(miniboardIndex);

    // Mark if the board is won/lost/tied
    if (boardStatus == toMove) {
      canonical[miniboardIndex * 22 + 18] = 1;

      // Mark every spot
      for (int spotIndex = 0; spotIndex < 9; spotIndex++) {
        canonical[miniboardIndex * 22 + spotIndex * 2] = 1;
      }
    }

    else if (boardStatus == 3) {
      canonical[miniboardIndex * 22 + 20] = 1;
    }

    else if (boardStatus == 2 / toMove) {
      canonical[miniboardIndex * 22 + 19] = 1;

      // Mark every spot
      for (int spotIndex = 0; spotIndex < 9; spotIndex++) {
        canonical[miniboardIndex * 22 + spotIndex * 2 + 1] = 1;
      }
    }

    else {
      for (int spotIndex = 0; spotIndex < 9; spotIndex++) {
        int spotStatus = getPosition(miniboardIndex, spotIndex);

        if (spotStatus == toMove) {
          canonical[miniboardIndex * 22 + spotIndex * 2] = 1;
        }

        // If the spot belongs to the other player
        else if (spotStatus == 2 / toMove) {
          canonical[miniboardIndex * 22 + spotIndex * 2 + 1] = 1;
        }

        // Else neither owns it, zero is default
      }
    }

    // Mark if this board is legal to move in
    if (boardStatus == 0 &&
        (requiredBoard == miniboardIndex || requiredBoard == -1)) {
      canonical[miniboardIndex * 22 + 21] = 1;
    }
  }

  vector<int> result;

  for (int i = 0; i < 199; i++) {
    result.push_back(canonical[i]);
  }

  return result;
}

nnInput GameState::getNNInput() {
  nnInput result;

  int toMove = getToMove();
  int requiredBoard = getRequiredBoard();

  for (int miniboardIndex = 0; miniboardIndex < 9; miniboardIndex++) {
    int boardStatus = getBoardStatus(miniboardIndex);

    // Mark if the board is won/lost/tied
    if (boardStatus == toMove) {
      result.board[miniboardIndex * 22 + 18] = 1;

      // Mark every spot
      for (int spotIndex = 0; spotIndex < 9; spotIndex++) {
        result.board[miniboardIndex * 22 + spotIndex * 2] = 1;
      }
    }

    else if (boardStatus == 3) {
      result.board[miniboardIndex * 22 + 20] = 1;
    }

    else if (boardStatus == 2 / toMove) {
      result.board[miniboardIndex * 22 + 19] = 1;

      // Mark every spot
      for (int spotIndex = 0; spotIndex < 9; spotIndex++) {
        result.board[miniboardIndex * 22 + spotIndex * 2 + 1] = 1;
      }
    }

    else {
      for (int spotIndex = 0; spotIndex < 9; spotIndex++) {
        int spotStatus = getPosition(miniboardIndex, spotIndex);

        if (spotStatus == toMove) {
          result.board[miniboardIndex * 22 + spotIndex * 2] = 1;
        }

        // If the spot belongs to the other player
        else if (spotStatus == 2 / toMove) {
          result.board[miniboardIndex * 22 + spotIndex * 2 + 1] = 1;
        }

        // Else neither owns it, zero is default
      }
    }

    // Mark if this board is legal to move in
    if (boardStatus == 0 &&
        (requiredBoard == miniboardIndex || requiredBoard == -1)) {
      result.board[miniboardIndex * 22 + 21] = 1;
    }
  }

  vector<int> validMoves = getAllPossibleMovesVector();
  for (int i = 0; i < 81; i++) {
    result.valid[i] = validMoves[i];
  }

  return result;
}

bitset<199> GameState::getCanonicalBoardBitset() {
  bitset<199> canonical;

  int toMove = getToMove();
  int requiredBoard = getRequiredBoard();

  for (int miniboardIndex = 0; miniboardIndex < 9; miniboardIndex++) {
    int boardStatus = getBoardStatus(miniboardIndex);

    // Mark if the board is won/lost/tied
    if (boardStatus == toMove) {
      canonical[miniboardIndex * 22 + 18] = 1;

      // Mark every spot
      for (int spotIndex = 0; spotIndex < 9; spotIndex++) {
        canonical[miniboardIndex * 22 + spotIndex * 2] = 1;
      }
    }

    else if (boardStatus == 3) {
      canonical[miniboardIndex * 22 + 20] = 1;
    }

    else if (boardStatus == 2 / toMove) {
      canonical[miniboardIndex * 22 + 19] = 1;

      // Mark every spot
      for (int spotIndex = 0; spotIndex < 9; spotIndex++) {
        canonical[miniboardIndex * 22 + spotIndex * 2 + 1] = 1;
      }
    }

    else {
      // Mark the spots

      for (int spotIndex = 0; spotIndex < 9; spotIndex++) {
        int spotStatus = getPosition(miniboardIndex, spotIndex);

        if (spotStatus == toMove) {
          canonical[miniboardIndex * 22 + spotIndex * 2] = 1;
        }

        // If the spot belongs to the other player
        else if (spotStatus == 2 / toMove) {
          canonical[miniboardIndex * 22 + spotIndex * 2 + 1] = 1;
        }

        // Else neither owns it, zero is default
      }
    }

    // Mark if this board is legal to move in
    if (boardStatus == 0 &&
        (requiredBoard == miniboardIndex || requiredBoard == -1)) {
      canonical[miniboardIndex * 22 + 21] = 1;
    }
  }

  return canonical;
}

GameState boardVector2GameState(vector<int> board) {
  GameState result;
  for (int miniboardIndex = 0; miniboardIndex < 9; miniboardIndex++) {
    for (int spotIndex = 0; spotIndex < 9; spotIndex++) {
      if (board[miniboardIndex * 22 + spotIndex * 2] == 1) {
        result.setPosition(miniboardIndex, spotIndex, 1);

      } else if (board[miniboardIndex * 22 + spotIndex * 2 + 1] == 1) {
        result.setPosition(miniboardIndex, spotIndex, 2);
      }
    }
  }

  result.updateMiniboardStatus();

  return result;
}
