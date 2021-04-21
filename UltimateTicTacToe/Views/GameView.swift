//
//  GameView.swift
//  UltimateTicTacToe
//
//  Created by Mason Raffo on 4/14/21.
//

import SwiftUI


struct GameView: View {
    /// The amount of padding from the edge of the display to the main board
    var po: Int = 5
    
    /// The amount of padding from the edge of the main board to the miniboard
    var p: Int = 10
    
    /// The current state of the game and it's visual representation
    @State var game:Game = Game();
    
    /// The Monte Carlo Search Tree behind the AI agent.
    @State var mcts:MCTS = MCTS();
    
    @Binding var settings: GameSettings
    
    var body: some View {
        ZStack {
            let textStatus = ["", "X", "O", "T"]
            let textColor = [Color.white, Color.red, Color.blue, Color.black]
            
            BoardView(p: p, po: po, game: $game, settings: $settings)
            
            
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
                        if game.boardStates[boardY * 3 + boardX] == 0 {
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
            
            GeometryReader { geometry in
                // Amount of space above the board
                let offsetAmountY: Int = (Int(geometry.size.height) - Int(geometry.size.width)) / 2
                
                VStack {
                    
                    // Width of a miniboard
                    let h: Int = (Int(geometry.size.width) - 2 * po) / 3
                    
                    VStack {
                        ForEach(0..<3) { boardX in
                            HStack {
                                ForEach(0..<3) { boardY in
                                    Text(textStatus[game.boardStates[boardY + boardX * 3]])
                                        .fontWeight(.bold)
                                        .font(.system(size:CGFloat(h)))
                                        .frame(width: CGFloat(h - p / 2), height: CGFloat(h), alignment: .center)
                                        .scaledToFit()
                                        .foregroundColor(textColor[game.boardStates[boardY + boardX * 3]])
                                    
                                }
                            }
                            
                        }
                    }.padding(CGFloat(po))
                    
                }.offset(y: CGFloat(offsetAmountY))
                
                
            }
            
            
            // User interface
            GeometryReader { geometry in
                let offsetAmountY: Int = (Int(geometry.size.height) - Int(geometry.size.width)) / 2

                VStack {
                    Text(game.player2Time)
                        .position(x: 100, y: CGFloat(offsetAmountY - 50))
                    
                    Text(game.player1Time)
                        .position(x: 100, y: 250)


                }
            }
        }
    }
}

struct GameView_Previews: PreviewProvider {
    static var previews: some View {
        GameView(settings: .constant(GameSettings()))
    }
}
