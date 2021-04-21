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
    
    func infer(board: Data, valid: Data) ->CnnOutput? {
        let policyOutput: Tensor
        let valueOutput: Tensor
        
        do {
            try interpreter.copy(board, toInputAt: 0)

            try interpreter.copy(valid, toInputAt: 1)

            
            try interpreter.invoke()
            
            policyOutput = try interpreter.output(at: 0)
            valueOutput = try interpreter.output(at: 1)
            
        } catch let error {
            print("Failed to invoke the interpreter with error: \(error.localizedDescription)")
            return nil
        }
        
        var results = CnnOutput()
        
        let value = UnsafeMutablePointer<Float32>.allocate(capacity: 1)
        let policy = UnsafeMutablePointer<Float32>.allocate(capacity: 81)
        
        let valueBuffer = UnsafeMutableBufferPointer<Float32>(start: value, count: 1)
        let policyBuffer = UnsafeMutableBufferPointer<Float32>(start: policy, count: 81)

        let _ = valueOutput.data.copyBytes(to: valueBuffer)
        let _ = policyOutput.data.copyBytes(to: policyBuffer)
        
        results.value = value.pointee
        memcpy(&results.policy, policy, 81 * MemoryLayout<Float32>.size)
        
        value.deallocate()
        policy.deallocate()
        
//        let resultsPolicy = UnsafeMutablePointer<Float32>.allocate(capacity: 81)
//        let resultsValue = UnsafeMutablePointer<Float32>.allocate(capacity: 1)
//
//        let policyBuffer = UnsafeMutableBufferPointer(start: resultsPolicy, count: 81)
//        let resultsBuffer = UnsafeMutableBufferPointer(start: resultsValue, count: 1)
//
//        let policyResultsQuantized = [UInt8](policyOutput.data)
//        let valueResultsQuantized = [UInt8](valueOutput.data)
//
//        let resultsUnquantized: [Float]
//
//        var results:CnnOutput = CnnOutput()
//
//        if let policyQuantization = policyOutput.quantizationParameters {
//            resultsUnquantized = policyResultsQuantized.map {
//                policyQuantization.scale * Float(Int($0) - policyQuantization.zeroPoint)
//            }
//
//            resultsPolicy.initialize(from: resultsUnquantized, count: 81)
//
//
//        }
//
//        else {
////            resultsUnquantized = [Float](policyOutput.data
//        }
//
//        if let valueQuantization = valueOutput.quantizationParameters {
//            let valueUnquantized = valueResultsQuantized.map {
//                valueQuantization.scale * Float(Int($0) - valueQuantization.zeroPoint)
//            }
//
//            resultsValue.initialize(from: valueUnquantized, count: 1)
//
//
//
//        } else {
//            policyOutput.data.copyBytes(to: resultsBuffer)
//        }
//
//        memcpy(&results.policy, resultsPolicy, MemoryLayout<Float32>.size * 81)
//
//        memcpy(&results.value, resultsValue, MemoryLayout<Float32>.size * 1)


        return results
    }
}
