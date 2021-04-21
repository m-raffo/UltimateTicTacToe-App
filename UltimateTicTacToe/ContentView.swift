//
//  ContentView.swift
//  UltimateTicTacToe
//
//  Created by Mason Raffo on 4/12/21.
//

import SwiftUI

struct ContentView: View {
    
    /// Set to true when the GameView is showing
    @State private var showingGame = false
    
    @State var mctsSims: Double = 1500
    
    @State var aiOpponent = true
    
    @State var settings: GameSettings = GameSettings()
    
    var body: some View {
        if showingGame {
            GameView(settings: $settings)
        } else {
            MainMenuView(showingGame: $showingGame, settings: $settings)
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
