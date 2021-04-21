//
//  MCTS.swift
//  UltimateTicTacToe
//
//  Created by Mason Raffo on 4/18/21.
//

import Foundation

struct MCTS {
    let mcts = CreateMCTS(1, 1);
    
    let model = ModelDataHandler();
    
    /**
     Performs search iterations.
     
     - Parameter num: The number of searches to perform
     */
    func PerformIterations(num: Int) -> Void {
        var toNN: CnnInput
        var boardData: Data
        var validsData: Data
        
        // Loop through every MCTS sim
        for _ in 1...num {
            toNN = SearchPreNN(mcts)
            
            // Only perform a neural network evaluation if one is needed
            if (IsEvalNeeded(mcts) == 1) {
                
                // Copy data
                boardData = Data(bytes: &toNN.board, count: MemoryLayout<Float>.size * 199)
                validsData = Data(bytes: &toNN.valid, count: MemoryLayout<Float>.size * 81)
                
                // Get result
                guard let result = model?.infer(board: boardData, valid: validsData) else {
                    print("Model returned nil")
                    return
                }
                
                SearchPostNN(mcts, result)
            }
            
        }
        
    }
    
    /**
     Gets the current best action.
     - Returns: The action index in 0...80
     */
    func GetBestMove() -> Int {
        return Int(GetBestAction(mcts))
    }
    
    /**
     Take the given action on the MCTS.
     - Parameter action: The action to be taken as an index between 0...80
     */
    func TakeAction(action: Int) -> Void {
        TakeActionMCTS(mcts, Int32(action))
    }
}

