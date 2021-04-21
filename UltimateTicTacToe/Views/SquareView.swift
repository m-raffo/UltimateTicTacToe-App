//
//  SquareView.swift
//  UltimateTicTacToe
//
//  Created by Mason Raffo on 4/14/21.
//

import SwiftUI

struct SquareView: View {
    var index: Int
    var size: CGFloat
    var accentColor: Color = Color(red: 176/255, green: 206/255, blue: 255/255)
    var backColor: Color = Color(white: 0.96)
    
    static var feedbackGenerator: UIImpactFeedbackGenerator = UIImpactFeedbackGenerator(style: .light)
    
    @Binding var game:Game
    
    @Binding var settings: GameSettings
    
    
    @State var currentText: String = ""
    
    static let pieceStrings = [0: " ", 1: "X", 2: "O"]
    
    var body: some View {
        Button(action: {
            print("\(index) pressed")
            if game.clock.currentPlayer == 0 {
                game.clock.currentPlayer = 1
                game.clock.startClock()
                
                Timer.scheduledTimer(withTimeInterval: 0.1, repeats: true) { timer in
                    game.UpdateTimers()
                }

            }
            
            if !game.aiMove {
                
                let b = Int32(index / 9)
                let p = Int32(index % 9)
                
                if IsValidMove(game.game, b, p) == 1 {
                    SquareView.feedbackGenerator.impactOccurred()
                    
                    game.aiMove = settings.aiOpponent
                    game.move(board: Int(b), piece: Int(p))
                    
                    game.board[index] = Int(GetPosition(game.game, b, p))
                    print(game.board[index])
                    game.requiredBoard = Int(GetRequiredBoard(game.game))
                    
                    
                    if game.requiredBoard > -1 {
                        game.bgColors = [Color](repeating: Color.white, count: 9)
                        game.bgColors[game.requiredBoard] = game.CurrentColor()
                    } else {
                        game.bgColors = [Color](repeating: game.CurrentColor(), count: 9)
                    }
                    
                    if settings.aiOpponent {
                        // Start the computer thinking about its response
                        DispatchQueue.global(qos: .userInitiated).async {
                            game.mcts.PerformIterations(num: Int(settings.MCTSsims))
                            
                            let compAction = game.mcts.GetBestMove()
                            
                            let b: Int = compAction / 9
                            let p: Int = compAction % 9
                            
                            game.move(board: b, piece: p)
                            
                            SquareView.feedbackGenerator.impactOccurred()
                            
                            
                            game.board[compAction] = Int(GetPosition(game.game, Int32(b), Int32(p)))
                            game.aiMove = false
                            game.requiredBoard = Int(GetRequiredBoard(game.game))
                            
                            if game.requiredBoard > -1 {
                                game.bgColors = [Color](repeating: Color.white, count: 9)
                                game.bgColors[game.requiredBoard] = game.CurrentColor()
                            } else {
                                game.bgColors = [Color](repeating: game.CurrentColor(), count: 9)
                            }
                        }
                    }
                    
                    
                }
            }
            
            
            
        }) {
            if game.boardStates[Int(index / 9)] == 0 {
                Text(SquareView.pieceStrings[game.board[index]] ?? " ")
                    .fontWeight(.bold)
                    .font(.system(size:size * 0.8))
                    .frame(minWidth: size * 0.8, minHeight: size * 0.8)
                    .scaledToFit()
                    .foregroundColor(SquareView.pieceStrings[game.board[index]] ?? " " == "X" ? game.theme.xColor : game.theme.oColor)
            }
            
            
            //                .background(index % 2 == 0 ? accentColor : backColor)
        }.frame(minWidth: size, minHeight: size)
        
    }
}

struct SquareView_Previews: PreviewProvider {
    static var previews: some View {
        SquareView(index: 1, size: 60, game: .constant(Game()), settings: .constant(GameSettings()))
    }
}
