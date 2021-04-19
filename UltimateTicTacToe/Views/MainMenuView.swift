//
//  MainMenuView.swift
//  UltimateTicTacToe
//
//  Created by Mason Raffo on 4/14/21.
//

import SwiftUI

struct MainMenuView: View {
    @Binding var showingGame: Bool
    @Binding var mctsSims: Double
        
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
                    showingGame = true
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
                    
                    let game = CreateGameState();
                    let mcts = CreateMCTS(1, 1);
                    
                    GameStateMove(game, 0, 1);
                    GameStateMove(game, 1, 7);
                    
                    StartNewSearch(mcts, game);
                    
                    let nnInput = SearchPreNN(mcts);
                    
                    print(nnInput.board);
                    
                    ReleaseMCTS(mcts);
                    ReleaseGameState(game);
                                        
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
                
                Slider(value: $mctsSims, in: 200...10000, step:100)
                
                Spacer()

            }


        }

    }
}

struct MainMenuView_Previews: PreviewProvider {
    static var previews: some View {
        MainMenuView(showingGame: .constant(true), mctsSims: .constant(500))
    }
}
