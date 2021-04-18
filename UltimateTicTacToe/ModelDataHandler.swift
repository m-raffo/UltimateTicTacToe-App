//
//  ModelDataHandler.swift
//  UltimateTicTacToe
//
//  Created by Mason Raffo on 4/18/21.
//

import Foundation
import TensorFlowLite

class ModelDataHandler {
    
    var interpreter: Interpreter
    
    init?() {
        
        // Load the model
        guard let modelPath = Bundle.main.path(forResource: "model", ofType: "tflite")
        else {
            print("Failed to load the model")
            return nil
        }
        
        do {
            interpreter = try Interpreter(modelPath: modelPath)
            
            try interpreter.allocateTensors()
        } catch let error {
            print("Failed to create the interpreter with error \(error.localizedDescription)")
            return nil
        }
    }
}
