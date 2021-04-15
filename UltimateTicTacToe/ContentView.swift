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
    
    var body: some View {
        if showingGame {
            GameView()
        } else {
            MainMenuView(showingGame: $showingGame)
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
