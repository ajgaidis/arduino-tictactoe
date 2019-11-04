/**
 * File: tictactoe.ino
 * Description: the game logic for tictactoe
 * 
 * Author: Alexander J. Gaidis (agaidis)
 * E-mail: alexander_gaidis@brown.edu
 */

#include "board.cpp" /* and pins.h, game_config.h */

/* Define the main game board object */
Board game = Board();

/* Define some time globals to use for debouncing */
unsigned long previousMillis = 0;
const long interval = 15;

/**
 * Define arrays of LED pins for easy selection during 
 * state display!
 */
int red_led_cols[3] = {ledC0R, ledC1R, ledC2R};
int blu_led_cols[3] = {ledC0B, ledC1B, ledC2B};
int led_rows[3] = {ledR0, ledR1, ledR2};

/**
 * Define arrays of buttons for easy selection during
 * user move selection!
 */
int button_cols[3] = {buttonC0, buttonC1, buttonC2};
int button_rows[3] = {buttonR0, buttonR1, buttonR2};


/**
 *  Setup handles initializing pins and setting 
 *  the baud-rate of the serial port.
 */
void setup() {
  pinMode(buttonR0, OUTPUT);
  pinMode(buttonR1, OUTPUT);
  pinMode(buttonR2, OUTPUT);

  pinMode(buttonC0, INPUT);
  pinMode(buttonC1, INPUT);
  pinMode(buttonC2, INPUT);

  pinMode(ledR0, OUTPUT);
  pinMode(ledR1, OUTPUT);
  pinMode(ledR2, OUTPUT);

  pinMode(ledC0R, OUTPUT);
  pinMode(ledC0B, OUTPUT);
  pinMode(ledC1R, OUTPUT);
  pinMode(ledC1B, OUTPUT);
  pinMode(ledC2R, OUTPUT);
  pinMode(ledC2B, OUTPUT);
  Serial.begin(9600);
}

/**
 *  Arduino's standard loop function :)
 */
void loop() {
  if (game.cur_move == X) { /* AI move */
    if (game.move_count == 0) {
      /* The AI is going first so take the middle */
      game.make_move(1, 1);
    } else if (game.move_count == 1) {
      /* AI goes second so try to take the middle */
      if (game.is_valid_move(1, 1)) {
        game.make_move(1, 1); 
      } else {  /* take a corner */
        game.make_move(random(0, 2) * 2, random(0, 2) * 2);
      }
    } else {
       Board ret = minimax(game, minimax_depth, true);
       game.make_move(ret.best_row, ret.best_col);
    }
  } else {
    /* Human move */
    read_user_move();
  }
  
  display_state();
}

/**
 * Performs the minimax algorithm to calculate the AI's
 * next move.
 * 
 * Params:
 *  - b (Board): a game board object
 *  - depth (int): the depth to perform the algorithm to
 *  - is_maximize (bool): true if the current player is
 *                        the AI and is trying to maximize
 *                        their score, else false                       
 * 
 * Returns:
 *  a Board with its heuristic, best_row, and best_col
 *  attributes set according the best move for the AI
 */
Board minimax(Board b, int depth, bool is_maximize) {
  if (depth == 0 || b.is_terminal()) {
    b.calculate_heuristic();
    return b;
  }

  if (is_maximize) { /* Maximizing player (this is X, the AI) */
    int tmp_heuristic = -32768; /* Smallest 16-bit # */
    double_iter(row, col)
      if (b.is_valid_move(row, col)) {

        /* 1. Make a move (i.e. generate a child state) */
        b.board[row][col] = X; 
        /* 2. Make the recursive minimax call */
        int child_heuristic = minimax(b, depth - 1, false).heuristic;
        /* 3. If the heuristic is better, update the heuristic */
        /*    and the board's internal state*/
        if (tmp_heuristic < child_heuristic) {
          tmp_heuristic = child_heuristic;
          b.best_row = row; b.best_col = col; 
        }
        /* 4. Undo the move made (i.e. restore the parent state) */
        b.undo_move(row, col);
        
      }
    /* 5. Update the board with the best heuristic value*/
    b.heuristic = tmp_heuristic;
    return b;
    
  } else { /* Minimizing player (this is O, the human) */
    int tmp_heuristic = 32767; /* Largest 16-bit # */
    double_iter(row, col)
      if (b.is_valid_move(row, col)) {
        
        b.board[row][col] = O;
        int child_heuristic = minimax(b, depth - 1, true).heuristic;
        if  (tmp_heuristic > child_heuristic) {
          tmp_heuristic = child_heuristic;
          b.best_row = row; b.best_col = col;
        } 
        b.undo_move(row, col);
        
      }
    b.heuristic = tmp_heuristic;
    return b;
  }
}

/**
 * Reads the user's (human's) move that they input through
 * button presses.
 */
void read_user_move() {
  /*
   * These variables aid in debouncing. If I just went
   * off the time difference for debouncing, I wouldn't
   * be sure what button was pressed, so if two buttons
   * are pressed in rapid succession the first button
   * press would effectively debounce the second which
   * is incorrect.
   */
  static int last_row_press = 0;
  static int last_col_press = 0;

  /*
   * Time debouncing; the following three lines ensure
   * one can only enter the meat of this function each 
   * given time interval.
   */
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    /*
     * The button rows (output pins) are iteratively
     * written to high and then the button columns
     * (input pins) are are iteratively read to see
     * if they have been pressed.
     */
    iter(row) {
      digitalWrite(button_rows[row], HIGH);
      iter(col) {
        if (digitalRead(button_cols[col]) == HIGH) {
          if (last_col_press == button_cols[col] && 
              last_row_press == button_rows[row]) {
            game.make_move(row, col);
            return;
          } else {
            last_col_press = button_cols[col];
            last_row_press = button_rows[row];
          }
        }
      }
      digitalWrite(button_rows[row], LOW);
    }
    
  }
}

/**
 *  Lights up the LEDs corresponding to the given
 *  board of the game Board object.
 */
void display_state() {
  iter(row) {
    iter(col) {
      int token = game.board[row][col];
      if (token == X) {
        digitalWrite(red_led_cols[col], LOW);
      } else if (token == O) {
        digitalWrite(blu_led_cols[col], LOW);
      } else {
        digitalWrite(red_led_cols[col], HIGH);
        digitalWrite(blu_led_cols[col], HIGH);
      }
    }
    digitalWrite(led_rows[row], HIGH);
    delay(1);  // delays for one millisecond
    digitalWrite(led_rows[row], LOW);
    for (int col = 0; col < NCOLS; col++) {
      digitalWrite(red_led_cols[col], HIGH);
      digitalWrite(blu_led_cols[col], HIGH);
    }
  }
}

/**
 * Prints the representation of a game board, b, to Serial.
 * This function is mainly used for debugging as printing
 * slows things down quite a bit. The end result should
 * look something like:
 * 
 * +-------+
 * | X O X |
 * | - - O |
 * | X - - |
 * +-------+
 * 
 * Params:
 *  - b (Board): the Board object to print
 */
void print_board(Board b) {
  Serial.println("+-------+");
  for (int row = 0; row < NROWS; row++) {
    Serial.print("| ");
    for (int col = 0; col < NCOLS;  col++) {
      if (b.board[row][col] == X) {
        Serial.print("X ");
      } else if (b.board[row][col] == O) {
        Serial.print("O ");
      } else {
        Serial.print("- ");
      }
    }
    Serial.print("|\n");
  }
  Serial.println("+-------+");
}
