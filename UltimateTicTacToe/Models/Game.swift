//
//  Game.swift
//  UltimateTicTacToe
//
//  Created by Mason Raffo on 4/15/21.
//

import Foundation
import SwiftUI

struct Game {
    /// Array of all the spaces on the board
    var board: [Int] = [Int](repeating: 0, count: 81)
    
    /// Pointer to the GameState object
    var game: OpaquePointer? = CreateGameState()
    
    var mcts: MCTS = MCTS()
    
    /// True if AI is making a move and the player cannot make a move
    var aiMove: Bool = false
    
    /// Set to the required board or -1 if none
    var requiredBoard: Int = -1

    var bgColors: [Color] = [Color](repeating: Color.white, count: 9)
    
    func move(board: Int, piece: Int) {
        GameStateMove(game, Int32(board), Int32(piece))
        mcts.TakeAction(action: board * 9 + piece)
    }
    
}
