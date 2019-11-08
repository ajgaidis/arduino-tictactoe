Author: Alexander J. Gaidis (agaidis)
E-mail: alexander_gaidis@brown.edu


![Top-Down View](IMG_2069.png)

![Zoomed-In View](IMG_2073.png)

--[ Project Overview

The hardware for this project consists of:
o 1 Arduino
o 9 RGB LEDs
o 9 switches
o 9 diodes (1 for each switch)
o 3 resistors (1 for each switch column)
o a handful of wires
o 1 breadboard

Both the LEDs and the switches are arranged into 3x3 matrices to resemble a 
tic-tac-toe board with a 1-to-1 mapping between switches to LEDs.

The software for this project consists of 4 files:
o tictactoe.ino: this is the main file that controls the running game's logic
o board.cpp: this file contains the Board class that represents a game board
o game_config.h: this header file contains macros and game variables
o pins.h: this header file maps LEDs and switches to pins

To display the lights, I cycle through the rows of the LED matrix so that only
3 are lit up at a time. But, by doing this fast enough, all lights seem to be
lit up as the time between lighting up one row and another row is imperceivable.
For the switches I do a similar technique where I sense one column of switches
at a time for presses so that pressing multiple switches at a time doesn't
confuse the program.

To represent the game state (i.e. game board) I made a Board class in board.cpp
which holds a game's attributes such as the board and whose turn it is. There
are also a slew of functions in there for calculating heuristics for the AI,
making moves, undoing moves, and detecting wins, and more.

Within the tictactoe.ino file is the algorithm that controls the AI component
of this project. I use the minimax algorithm to choose the best move for the
AI based on a set of heuristics I developed. These heuristics revolve around
finding a winning board configuration or around the AI blocking the human 
player's moves to result in tie. In my first pass at coding the algorithm,
creating a child state involved creating a new board object. Space wasn't an
issue here, but time was and this approach caused my lights to not display
while minimax was crunching. To remedy this, I instead create one board object
whose 3x3 board array I edit and un-edit in-place. This saved a good amount of
time allowed my algorithm to be a bit cleaner as well.


--[ Design Decisions

The following notable design decision were made:

~ Gameplay ~
o The starting player alternates with each game (in the 1st game the AI starts)
o If a player wins, their color (X = red, O = blue) flashes 6 times
o If there is a tie, the flashing colors alternate; 3 flashes for each player
o The AI is X (red) and the human player is O (blue); this doesn't change

~ Hardware ~
o A 3x3 LED matrix is used to represent the game board
o A 3x3 switch matrix is used to select a move
o To allow for more digital pins, I hijacked some anolog pins to be digital :)
o Cables were managed by making small jumpers to span small breadboard gaps

~ Software ~
o AI uses the minimax algorithm (default depth of 3) to make its moves
o Game state is abstracted to a Board class 
o The AI doesn't use minimax for the first move to save on computation

--[ Install/Compile/Run

1. Download the source code (unzip if necessary).
2. Open tictactoe.ino in the Arduino IDE.
3. Make sure Arduino and appropriate hardware is hooked up and press `Upload`.

Note: all files should reside in the same directory (and be at the same level).


--[ Known Bugs

There are no bugs that I know of. Everything seems to work as intended.
