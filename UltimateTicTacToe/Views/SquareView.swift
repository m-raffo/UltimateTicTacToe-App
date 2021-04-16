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
    
    @Binding var board: [Int]
    @Binding var game:OpaquePointer?

    @State var currentText: String = ""
    
    static let pieceStrings = [0: " ", 1: "X", 2: "O"]
    
    var body: some View {
        Button(action: {
            print("\(index) pressed")
            let b = Int32(index / 9)
            let p = Int32(index % 9)
            
            if IsValidMove(game, b, p) == 1 {
                Move(game, b, p)
                board[index] = Int(GetPosition(game, b, p))
                
                let compMove = MinimaxSearchTimeMove(game, 6, 0)
                
                Move(game, compMove.board, compMove.piece);
                board[Int(compMove.board * 9 + compMove.piece)] = Int(GetPosition(game, compMove.board, compMove.piece))
            }
            
            
        }) {
            Text(SquareView.pieceStrings[board[index]] ?? " ")
                .fontWeight(.bold)
                .font(.system(size:size * 0.8))
                .frame(minWidth: size * 0.8, minHeight: size * 0.8)
                .scaledToFit()
                .foregroundColor(SquareView.pieceStrings[board[index]] ?? " " == "X" ? .red : .blue)
                
//                .background(index % 2 == 0 ? accentColor : backColor)
        }.frame(minWidth: size, minHeight: size)
        
    }
}

struct SquareView_Previews: PreviewProvider {
    static var previews: some View {
        SquareView(index: 1, size: 60, board: .constant(Array(repeating: 0, count: 5)), game: .constant(CreateCppClass()))
    }
}
