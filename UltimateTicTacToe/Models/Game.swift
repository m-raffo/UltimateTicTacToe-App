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
    
    var boardStates: [Int] = [Int](repeating: 0, count: 9)
    
    /// Pointer to the GameState object
    var game: OpaquePointer? = CreateGameState()
    
    var mcts: MCTS = MCTS()
    
    /// True if AI is making a move and the player cannot make a move
    var aiMove: Bool = false
    
    
    /// Set to the required board or -1 if none
    var requiredBoard: Int = -1
    
    var theme = Theme()

    var bgColors: [Color] = [Color](repeating: Color.white, count: 9)
    
    /**
     Gets the color to highlight the background of valid boards with.
     */
    func CurrentColor() -> Color {
        if GetToMove(game) == 1 {
            return theme.xBackground
        }
        
        return theme.oBackground
    }
    
    mutating func move(board: Int, piece: Int) {
        GameStateMove(game, Int32(board), Int32(piece))
        mcts.TakeAction(action: board * 9 + piece)
        
        self.boardStates[board] = Int(GetBoardStatus(game, Int32(board)))
    }
    
}
