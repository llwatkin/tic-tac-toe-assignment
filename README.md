# Tic Tac Toe

Base classes needed to implement tic tac toe have been provided for me. I added my logger from the previous assignment to help me test and debug. I worked on Windows.

## Implementing Tic Tac Toe

I first set up the board using the fact that the square pngs are 100x100 pixels. In each loop, I multiplied the row and column indices by 100 and added an offset to give them some margin at the top. I then followed the comments to fill in the implementation for other functions until I was able to place X's and O's down. I noticed that the first player would put down an O, so I fixed that, since player one is always the X in Tic Tac Toe. Finally, I filled in the checkForWinner function, using my logger to make sure it was working. I had trouble at first because I thought that `0 == 0 == 0` would evalute to `true`, out of habit with other programming languages. I eventually remembered that because `0 == 0` would evalute first to `true`, the next expression `true == 0` would evaluate to `false`, and was able to fix it. After implementing and testing the checkForWinner function, I added a global bool to TicTacToe.cpp called gameOver, which I used to prevent the placement of pieces after the game has ended. I also implemented the stateString function before realizing that we didn't need to do it yet.
