//
//  BoardView.swift
//  UltimateTicTacToe
//
//  Created by Mason Raffo on 4/14/21.
//

import SwiftUI

struct BoardView: View {
    /// The amount of padding from the edge of the display to the main board
    var p: Int
    
    /// The amount of padding from the edge of the main board to the miniboard
    var po: Int
    
    @Binding var board: [Int]
    @Binding var game:OpaquePointer?
    
    var body: some View {
        VStack {
            GeometryReader { geometry in
                let h: Int = (Int(geometry.size.width) - 2 * po) / 3
                let hi: Int = (h - 2 * p) / 3
                
                Color.white
                HStack {
                    Spacer()
                    VStack {
                        Spacer()
                        ForEach(0..<3) { boardY in
                            // Rows of boards
                            HStack (spacing: CGFloat(p * 2)) {
                                
                                
                                ForEach(0..<3) { boardX in
                                    // Stack of rows of spaecs (ie boards)
                                    VStack () {
                                        ForEach(0..<3) { y in
                                            HStack(spacing:0) {
                                                ForEach(0..<3) {x in
                                                    
                                                    SquareView(index: (boardY * 3 + boardX) * 9 + y * 3 + x, size: CGFloat(hi), board: $board, game: $game)
                                                }
                                                
                                            }
                                        }
                                        
                                    }
                                }
                            }.padding(CGFloat(p /  2))
                        }
                        Spacer()
                    }
                    Spacer()
                }

            }
            
        }
    }
}

struct BoardView_Previews: PreviewProvider {
    static var previews: some View {
        BoardView(p: 10, po: 20, board: .constant(Array.init(repeating: 0, count: 81)), game: .constant(CreateCppClass()))
    }
}
