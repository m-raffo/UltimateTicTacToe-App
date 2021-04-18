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
    
    @Binding var game:Game
    
    @State var currentText: String = ""
    
    static let pieceStrings = [0: " ", 1: "X", 2: "O"]
    
    var body: some View {
        Button(action: {
            print("\(index) pressed")
            if !game.aiMove {
                
                let b = Int32(index / 9)
                let p = Int32(index % 9)
                
                if IsValidMove(game.game, b, p) == 1 {
                    game.aiMove = true
                    // Make the move
                    Move(game.game, b, p)
                    game.board[index] = Int(GetPosition(game.game, b, p))
                    game.requiredBoard = Int(GetRequiredBoard(game.game))
                    
                    
                    if game.requiredBoard > -1 {
                        game.bgColors = [Color](repeating: Color.white, count: 9)
                        game.bgColors[game.requiredBoard] = Color(red: 36/255, green: 160/255, blue: 1, opacity: 0.5)
                    } else {
                        game.bgColors = [Color](repeating: Color(red: 36/255, green: 160/255, blue: 1, opacity: 0.5), count: 9)
                    }
                    
                    // Start the computer thinking about its response
                    DispatchQueue.global(qos: .userInitiated).async {
                        let compMove = MinimaxSearchTimeMove(game.game, 6, 0)
                        
                        Move(game.game, compMove.board, compMove.piece);
                        game.board[Int(compMove.board * 9 + compMove.piece)] = Int(GetPosition(game.game, compMove.board, compMove.piece))
                        game.aiMove = false
                        game.requiredBoard = Int(GetRequiredBoard(game.game))
                        
                        if game.requiredBoard > -1 {
                            game.bgColors = [Color](repeating: Color.white, count: 9)
                            game.bgColors[game.requiredBoard] = Color(red: 1, green: 87/255, blue: 87/255, opacity: 0.5)
                        } else {
                            game.bgColors = [Color](repeating: Color(red: 1, green: 87/255, blue: 87/255, opacity: 0.5), count: 9)
                        }
                    }
                }
            }
            
            
            
        }) {
            Text(SquareView.pieceStrings[game.board[index]] ?? " ")
                .fontWeight(.bold)
                .font(.system(size:size * 0.8))
                .frame(minWidth: size * 0.8, minHeight: size * 0.8)
                .scaledToFit()
                .foregroundColor(SquareView.pieceStrings[game.board[index]] ?? " " == "X" ? .red : .blue)
                
//                .background(index % 2 == 0 ? accentColor : backColor)
        }.frame(minWidth: size, minHeight: size)
        
    }
}

struct SquareView_Previews: PreviewProvider {
    static var previews: some View {
        SquareView(index: 1, size: 60, game: .constant(Game()))
    }
}
