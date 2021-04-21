//
//  Timer.swift
//  UltimateTicTacToe
//
//  Created by Mason Raffo on 4/21/21.
//

import Foundation

struct GameClock {
    // Used for elapsed time through system up-time
    private let info = ProcessInfo.processInfo
    
    init(duration: Double, increment: Double, delay: Double) {
        totalDuration = duration
        player1TimeRemaining = duration
        player2TimeRemaining = duration
        
        self.increment = increment
        self.delay = delay
        
        currentTurnStart = info.systemUptime
    }
    
    /// Value in seconds of the timer at the start of the game
    var totalDuration: Double
    
    /// Value in seconds added to the player's timers after their move
    var increment: Double
    
    /// Value in seconds before the player's timer starts counting down after their opponent's move
    var delay: Double
    
    /// Value in seconds remaining on the player's clock
    var player1TimeRemaining: Double
    var player2TimeRemaining: Double
    
    /// The clock that is currently counting down. 1 indicates player 1 and 2 indicates player 2
    var currentPlayer: Int = 0
    
    var currentTurnStart: TimeInterval
    
    /**
     Flips which player's turn it is and adjusts the time remaining accordingly.
     */
    mutating func switchTurn() {
        let interval = Double(info.systemUptime - currentTurnStart)
        
        // If time deducted
        if (interval >= delay) {
            if currentPlayer == 1 {
                player1TimeRemaining -= interval - delay
                player1TimeRemaining += increment
            } else if currentPlayer == 2 {
                player2TimeRemaining -= interval - delay
                player2TimeRemaining += increment
            }
        }
        
        // Switch the current player
        currentPlayer = 2 / currentPlayer
        
        // Reset the clock
        currentTurnStart = info.systemUptime
    }
    
    /**
     Start the clock without changing which player's turn it is.
     */
    mutating func startClock() {
        currentTurnStart = info.systemUptime
    }
    
    /**
     Pause the clock without changing which player's turn it is.
     */
    mutating func pauseClock() {
        let interval = Double(info.systemUptime - currentTurnStart)
        
        // If time deducted
        if (interval >= delay) {
            if currentPlayer == 1 {
                player1TimeRemaining -= interval - delay
                player1TimeRemaining += increment
            } else if currentPlayer == 2 {
                player2TimeRemaining -= interval - delay
                player2TimeRemaining += increment
            }
        }
    }
    
    /**
     Gets the time remaining on the given player's clock as a string in the format mm:ss if time remaining > 20; else format ss.hh
     */
    func timeFormatted(player: Int) -> String {
        var timeRemaining = player == 1 ? player1TimeRemaining : player2TimeRemaining
        
        if player == currentPlayer {
            let interval = Double(info.systemUptime - currentTurnStart)
            
            if (interval >= delay) {
                timeRemaining -= interval - delay
            }
            
        }
        
        if timeRemaining <= 0 {
            return ("00:00")
        }

        
        return timeFormat(timeRemaining: timeRemaining)
    }
    
    private func timeFormat(timeRemaining: Double) -> String {
        if timeRemaining <= 20 {
            return String(format: "%02.1f", timeRemaining)
        } else {
            return String(format: "%02d:%02d", Int(timeRemaining / 60), Int(timeRemaining) % 60)
        }
    }
}
