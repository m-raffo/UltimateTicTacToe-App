//
//  MainMenuView.swift
//  UltimateTicTacToe
//
//  Created by Mason Raffo on 4/14/21.
//

import SwiftUI

struct MainMenuView: View {
    @Binding var showingGame: Bool
    
    @Binding var settings: GameSettings
    
    let model: ModelDataHandler? = ModelDataHandler()
    
    var body: some View {
        
        NavigationView {
            VStack {
                Text("Tic Tac Toe")
                    .font(.system(size: 60))
                    .padding(.top, 20.0)
                Text("Ultimate")
                    .font(.system(size: 75))
                
                Spacer()
                
                Button(action: {
                    SquareView.feedbackGenerator.prepare()
                    showingGame = true
                    settings.aiOpponent = true
                }) {
                    Text("Computer\nvs\nPlayer")
                        .fontWeight(.bold)
                        .font(.title)
                        .foregroundColor(.red)
                        .multilineTextAlignment(.center)
                        .padding()
                        .scaledToFill()
                        .overlay(
                            RoundedRectangle(cornerRadius: 20)
                                .stroke(Color.red, lineWidth: 5)
                        )
                    
                }
                
                Spacer()
                
                Button(action:{
                    
                    SquareView.feedbackGenerator.prepare()
                    showingGame = true
                    settings.aiOpponent = false
                    
                    
                }) {
                    Text("Player\nvs\nPlayer")
                        .fontWeight(.bold)
                        .font(.title)
                        .foregroundColor(.red)
                        .multilineTextAlignment(.center)
                        .padding()
                        .overlay(
                            RoundedRectangle(cornerRadius: 20)
                                .stroke(Color.red, lineWidth: 5)
                        )
                }
                Spacer()
                
                Slider(value: $settings.MCTSsims, in: 200...10000, step:100)
                
                Spacer()
                
            }
            
            
        }
        
    }
}

struct MainMenuView_Previews: PreviewProvider {
    static var previews: some View {
        MainMenuView(showingGame: .constant(true), settings: .constant(GameSettings()))
    }
}
