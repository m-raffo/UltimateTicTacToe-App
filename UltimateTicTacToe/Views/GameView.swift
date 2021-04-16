//
//  GameView.swift
//  UltimateTicTacToe
//
//  Created by Mason Raffo on 4/14/21.
//

import SwiftUI

struct Checkerboard: Shape {
    let rows: Int
    let columns: Int
    
    func path(in rect: CGRect) -> Path {
        var path = Path()
        
        // figure out how big each row/column needs to be
        let rowSize = rect.height / CGFloat(rows)
        let columnSize = rect.width / CGFloat(columns)
        
        // loop over all rows and columns, making alternating squares colored
        for row in 0 ..< rows {
            for column in 0 ..< columns {
                if (row + column).isMultiple(of: 2) {
                    // this square should be colored; add a rectangle here
                    let startX = columnSize * CGFloat(column)
                    let startY = rowSize * CGFloat(row)
                    
                    let rect = CGRect(x: startX, y: startY, width: columnSize, height: rowSize)
                    path.addRect(rect)
                }
            }
        }
        
        return path
    }
}


struct GameView: View {
    /// The amount of padding from the edge of the display to the main board
    var po: Int = 5
    
    /// The amount of padding from the edge of the main board to the miniboard
    var p: Int = 10
    
    @State var game:OpaquePointer? = CreateCppClass();
    @State var board: [Int] = [Int](repeating: 0, count: 81);
        
    var body: some View {
        ZStack {
            BoardView(p: p, po: po, board: $board, game: $game)
            
            GeometryReader { geometry in
                // Amount of space above the board
                let offsetAmountY: Int = (Int(geometry.size.height) - Int(geometry.size.width)) / 2
                
                // Width of a miniboard
                let h: Int = (Int(geometry.size.width) - 2 * po) / 3
                
                // Width of space
                let hi: Int = (h - 2 * p) / 3
                
                let lineWidth = 5
                let miniWidth = 2
                
                // Main board
                Path { path in
                    // Vertical lines
                    path.move(to: CGPoint(x: po + h + lineWidth / 2 - lineWidth / 4, y: po + offsetAmountY))
                    path.addLine(to: CGPoint(x: po + h + lineWidth / 2 - lineWidth / 4, y: po + 3 * h + offsetAmountY))
                    
                    path.move(to: CGPoint(x: po + h * 2 + lineWidth / 2 - lineWidth / 4, y: po + offsetAmountY))
                    path.addLine(to: CGPoint(x: po + h * 2 + lineWidth / 2 - lineWidth / 4, y: po + 3 * h + offsetAmountY))
                    
                    // Horizontal lines
                    path.move(to: CGPoint(x: po, y: po + offsetAmountY + h  + lineWidth / 2 ))
                    path.addLine(to: CGPoint(x: po + 3 * h, y: po + offsetAmountY + h  + lineWidth / 2))
                    
                    path.move(to: CGPoint(x: po, y: po + offsetAmountY + h * 2 + lineWidth / 2  - lineWidth / 4))
                    path.addLine(to: CGPoint(x: po + 3 * h, y: po + offsetAmountY + h * 2 + lineWidth / 2 - lineWidth / 4 ))
                    

                }
                .stroke(Color.black, lineWidth: CGFloat(lineWidth))
                
                ForEach(0..<3) { boardX in
                    ForEach(0..<3) { boardY in
                        // Miniboard
                        Path { path in
                            let boardPixelX = po + p + h * boardX
                            let boardPixelY = po + p + h * boardY + offsetAmountY
                                + -2 * boardY + 2
                            // Vertical lines
                            path.move(to: CGPoint(x: boardPixelX + hi + miniWidth / 2 - miniWidth / 4, y: boardPixelY + 4))
                            path.addLine(to: CGPoint(x: boardPixelX + hi + miniWidth / 2 - miniWidth / 4, y: boardPixelY + 3 * hi - 4))
                            
                            path.move(to: CGPoint(x: boardPixelX + hi * 2 + miniWidth / 2 - miniWidth / 4, y: boardPixelY + 4))
                            path.addLine(to: CGPoint(x: boardPixelX + hi * 2 + miniWidth / 2 - miniWidth / 4, y: boardPixelY + 3 * hi - 4))
                            
                            // Horizontal lines
                            path.move(to: CGPoint(x: boardPixelX + 4, y: boardPixelY + hi + miniWidth / 2 - miniWidth / 4))
                            path.addLine(to: CGPoint(x: boardPixelX + 3 * hi - 4, y: boardPixelY + hi + miniWidth / 2 - miniWidth / 4))
                            
                            
                            path.move(to: CGPoint(x: boardPixelX  + 4, y: boardPixelY + hi * 2 + miniWidth / 2 - miniWidth / 4))
                            path.addLine(to: CGPoint(x: boardPixelX + 3 * hi - 4, y: boardPixelY + hi * 2 + miniWidth / 2 - miniWidth / 4))
                            

                        }
                        .stroke(Color.black, lineWidth: CGFloat(miniWidth))
                    }
                }
                
                
            }
        }
    }
}

struct GameView_Previews: PreviewProvider {
    static var previews: some View {
        GameView()
    }
}
