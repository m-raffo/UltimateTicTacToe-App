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
    
    @State var mctsSims: Double = 500
    
    var body: some View {
        if showingGame {
            GameView(mctsSims: $mctsSims)
        } else {
            MainMenuView(showingGame: $showingGame, mctsSims: $mctsSims)
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
