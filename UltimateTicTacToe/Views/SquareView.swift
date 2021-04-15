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

    @State var currentText: String = ""
    var body: some View {
        Button(action: {
            print("\(index) pressed")
            self.currentText = index % 2 == 0 ? "X" : "O"
            
        }) {
            Text(currentText)
                .fontWeight(.bold)
                .font(.system(size:size * 0.8))
                .frame(minWidth: size * 0.8, minHeight: size * 0.8)
                .scaledToFit()
                .foregroundColor(index % 2 == 0 ? .red : .blue)
                
//                .background(index % 2 == 0 ? accentColor : backColor)
        }.frame(minWidth: size, minHeight: size)
        
    }
}

struct SquareView_Previews: PreviewProvider {
    static var previews: some View {
        SquareView(index: 1, size: 60)
    }
}
