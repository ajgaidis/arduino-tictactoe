/**
 * File: board.cpp
 * Description: contains the Board class that 
 *              represents a tic-tac-toe game
 * 
 * Author: Alexander J. Gaidis (agaidis)
 * E-mail: alexander_gaidis@brown.edu
 */

#include "pins.h"
#include "game_config.h"

/**
 * A Board object represents a tic-tac-toe game
 */
class Board
{
  private:
     /**
       Checks whether a 1x3 array is a winning array for player.
       That is if all elements in the array equal player's pieces
       a win is deemed.
    
       Params:
        - player (int): either X or O
        - arr (1D array): the array to test for a win. This is
                          usually a slice of the larger game board
                          such as a row, column,  or diagonal.
       Returns:
        true if a player has filled up the entire array with their
    */
    bool check_array_for_win(int player, int arr[3]) {
      return (player == arr[0] &&
              player == arr[1] &&
              player == arr[2]);
    }

    /**
       Gets a board's row as specified by row_index. For example,
       consider the following where "-" represents possible moves.
    
       row_index == 0:  - - -
       row_index == 1:  - - -
       row_index == 2:  - - -
    
       Params:
        - row_index (int): the row to extract; must be [0, 3)
    
       Returns:
        A 1D array representing a row that contains
        3 values that are either X, O or EMPTY determined
        by the tic-tac-toe board passed in.
    */
    int *get_row(int row_index) {
      return board[row_index];
    }

    /**
       Gets a board's column as specified by col_index. For example,
       consider the following where "-" represents possible moves.
    
       col_index == 0   |   col_index == 1  |  col_index == 2
            -                     -                  -
            -                     -                  -
            -                     -                  -
    
       Params:
        - col_index (int): the column to extract; must be [0, 3)
    
       Returns:
        A 1D array representing a column that contains
        3 values that are either X, O or EMPTY determined
        by the tic-tac-toe board passed in.
    */
    int *get_col(int col_index) {
      static int col[3];
      iter(row) 
        col[row] = board[row][col_index];
      return col;
    }

    /**
       Gets a board's diagonals as specified by is_backward_diag.
       Consider the following where D represents the values that
       are extracted into the returned 1D array.
    
       is_backward_diag == true  |   is_backward_diag == false
                D - -                         - - D
                - D -                         - D -
                - - D                         D -  -
    
    
       Params:
        - is_backward_diag (bool): true if the diagonal returned
                                   should resemble a '\' else an
                                   '/' symbol
    
       Returns:
        A 1D array representing a diagonal that contains
        3 values that are either X, O or EMPTY determined
        by the tic-tac-toe board passed in.
    */
    int *get_diag(bool is_backward_diag) {
      static int diag[3];
      if (is_backward_diag) {
        diag[0] = board[0][0];
        diag[1] = board[1][1];
        diag[2] = board[2][2];
      } else {
        diag[0] = board[0][2];
        diag[1] = board[1][1];
        diag[2] = board[2][0];
      }
      return diag;
    }

    /**
     * Counts the number of empty spaces (value 0) 
     * in a 1x3 array.
     * 
     * Params:
     *  - arr (1x3 array): the array to check
     * 
     * Returns:
     *  the number of 0s (EMPTYs) in the array
     */
    int num_empty_in_arr(int arr[3]) {
      /* We can add booleans; true = 1, false = 0 */
      return ((arr[0] == EMPTY) +
              (arr[1] == EMPTY) + 
              (arr[2] == EMPTY));
    }

    /**
     * Counts the number of player moves (X or O)
     * in a 1x3 array.
     * 
     * Params:
     *  - arr (1x3 array): the array to check
     * 
     * Returns:
     *  the number of player moves in the array
     */
    int num_moves_in_arr(int player, int arr[3]) {
      /* We can add booleans; true = 1, false = 0 */
      return ((player == arr[0]) +
              (player == arr[1]) +
              (player == arr[2]));
    }

    /**
     * Returns 1 if there is a pair of matched
     * singles in an array. I  define matched
     * singles as something like [-XO] where
     * both players have a piece in the array.
     * 
     * Params:
     *  - arr (1x3 array): the array to check
     *
     * Returns:
     *  1 if there is a pair of matched singles,
     *  else 0
     */
    int has_matched_singles(int arr[3]) {
      return (num_moves_in_arr(X, arr) == 1 && 
             (num_moves_in_arr(O, arr) == 1));
    }

    /**
     * Returns 1 if there is a blocked double
     * in an array. I define blocked double as
     * something like [OOX] or [OXO] where one
     * player owns two spots and the other player
     * owns the remaining spot.
     * 
     * Params:
     *  - player (int): the player to check for
     *                  the majority of moves from
     *                  i.e. if player is O then
     *                  [OOX] would return 1. 
     * - arr (1x3 array): the array to check
     * 
     * Returns:
     *  1 if there is a blocked double, else 0
     */
    int has_blocked_double(int player, int arr[3]) {
      return (num_moves_in_arr(player, arr) == 2 && 
             (num_moves_in_arr(other_player(player), arr) == 1));
    }

    /**
     * Counts the number of matched singles 
     * (as defined above) on this Board.
     */
    int get_num_matched_singles() {
      int total = 0;
      
      /* Check rows for singles */
      iter(row)
        total += has_matched_singles(get_row(row));
    
      /* Check cols for singles */
      iter(col)
        total += has_matched_singles(get_row(col));

      /* Check diagonals for singles */
      total += has_matched_singles(get_diag(false));
      total += has_matched_singles(get_diag(true));
      
      return total;
    }

    /**
     * Counts the number of blocked doubles 
     * (as defined above) on this Board.
     */
     int get_num_blocked_doubles(int player) {
       int total = 0;
      
       /* Check rows for singles */
       iter(row)
         total += has_blocked_double(player, get_row(row));
    
       /* Check cols for singles */
       iter(col)
         total += has_blocked_double(player, get_row(col));

       /* Check diagonals for singles */
       total += has_blocked_double(player, get_diag(false));
       total += has_blocked_double(player, get_diag(true));
      
       return total;
     }

    
  public:
    /* game state vars */
    int board[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    int cur_move = X;
    int starter = X;
    int move_count = 0;
    
    /* minimax vars */
    int heuristic = 0;
    int best_row = 0;
    int best_col = 0;
    
    /**
       A move is valid if the row and column of the move fall
       within the standard 3x3 tic-tac-toe board AND if no player
       has already placed a move there.
    
       Params:
        - row (int): the row position of the move to test, bounded by [0, 2]
        - col (int): the column position of the move to test, bounded by [0, 2]
    
       Returns:
        true if the specified move is playable, else false
    */
    bool is_valid_move(int row, int col) {
      return board[row][col] == EMPTY;
    }

    /**
       Checks whether the player has won the game on a given board.
    
       Params:
        - player (int): either X or O; the player to win-check
    
        Returns:
          true if there is a win by player, else false
    */
    bool check_win(int player) {
      /* Check rows for win */
      iter(row)
        if (check_array_for_win(player, get_row(row))) return true;
    
      /* Check cols for win */
      iter(col)
        if (check_array_for_win(player, get_col(col))) return true;
    
      /* Check diagonals for win */
      if (check_array_for_win(player, get_diag(false)) ||
          check_array_for_win(player, get_diag(true))) return true;
    
      /* No wins */
      return false;
    }

    /**
     * Checks whether the current game board is tied.
     * 
     * Returns:
     *  true if the game board is tied, else false
     */
     bool is_tie() {
       /*
        * Ties are checked for this way because minimax doesn't
        * increase the move_count when it makes children
        */
       double_iter(r, c)
         if (board[r][c] == EMPTY)
           return false;
       return true;
     }

    /**
     * Checks whether the game is in a terminal state.
     * 
     * Returns:
     *  true if the game has finished for a given snapshot
     *  Board, else false
    */
    bool is_terminal() {
      if (check_win(O) || check_win(X))
        return true;
      return is_tie();
    }

    /**
     * Resets this Board to its initial state.
     * The starting player will change with each
     * call to this function.
     */
    void reset_game() {
      double_iter(row, col)
          board[row][col] = EMPTY;
      starter = other_player(starter);
      cur_move = starter;
      move_count = 0;
    }

   /**
     Places the player piece in row, col by updating the state.

     Params:
      - player (int): either X or O
      - row (int): the row to place the move bounded by [0-2]
      - col (int): the column to place the move bounded by [0-2]
    */
    void make_move(int row, int col) {
      if (is_valid_move(row, col)) {
        
        board[row][col] = cur_move;
        move_count++;
        
        if (check_win(cur_move)) {
          int i = 0;
          while (i < 6) {
            display_win();
            i++;
          }
          reset_game();
        } else if (is_tie())  {
            int i = 0;
            while (i < 6) {
              display_win();
              i++;
              /* Get the lights to alternate */
              /* to show that there was no   */
              /* winner.                     */
              cur_move = other_player(cur_move);
            }
            reset_game();
        } else { /*if no win/tie just switch turn*/
          cur_move = other_player(cur_move);
        }
      }
    }

    /**
     * Sets a position in this Board to empty
     * 
     * Params:
     *  - row (int): the row specification [0, 2]
     *  - col (int): the column specification [0, 2]
     */
    void undo_move(int row, int col) {
      board[row][col] = EMPTY;
    }

    /**
     * Used by the minimax algorithm. Calculates a heuristic
     * value for this Board and sets the heuristic attribute.
     * 
     * NOTE: heuristics calculations were made to support
     *       single AI functionality (move X). This 
     *       function would need to be edited in the event 
     *       that two AIs want to play each other.
     */
    void calculate_heuristic() {
        if (check_win(O)) {
          heuristic -= 100;
        } else if (check_win(X)) {
          heuristic += 100;
        } else {
          /*
           * If there is no win for either side, make
           * the AI shoot for blocking the other
           * player's two-in-a-row/col/diag or just 
           * getting a piece in each row/col/diag where
           * an opponents piece might be!
           */
          heuristic += get_num_matched_singles();
          heuristic += get_num_blocked_doubles(O) * 10;
        }
    }

    /**
     * Displays an LED pattern for celebrating a win :)
     */
    void display_win() {
      /* Reset the LED pins */
      digitalWrite(ledC0R, HIGH);
      digitalWrite(ledC1R, HIGH);
      digitalWrite(ledC2R, HIGH);
      digitalWrite(ledC0B, HIGH);
      digitalWrite(ledC1B, HIGH);
      digitalWrite(ledC2B, HIGH);
      digitalWrite(ledR0, LOW);
      digitalWrite(ledR1, LOW);
      digitalWrite(ledR2, LOW);
      /* Display the winner's pattern */
      if (cur_move == X) {
        /* Red LEDS are turned on */
        digitalWrite(ledC0R, LOW);
        digitalWrite(ledC1R, LOW);
        digitalWrite(ledC2R, LOW);
        digitalWrite(ledC0B, HIGH);
        digitalWrite(ledC1B, HIGH);
        digitalWrite(ledC2B, HIGH);
      } else {
        /* Blue LEDS are turned on */
        digitalWrite(ledC0R, HIGH);
        digitalWrite(ledC1R, HIGH);
        digitalWrite(ledC2R, HIGH);
        digitalWrite(ledC0B, LOW);
        digitalWrite(ledC1B, LOW);
        digitalWrite(ledC2B, LOW);
      }
      /* 
       *  At this point nothing else should 
       *  be running or trying to run, so it 
       *  is OK to use delay
       */
      digitalWrite(ledR0, HIGH);
      delay(75); 
      digitalWrite(ledR0, LOW);
      digitalWrite(ledR1, HIGH);
      delay(75);
      digitalWrite(ledR1, LOW);
      digitalWrite(ledR2, HIGH);
      delay(75);
      digitalWrite(ledR2, LOW);
      delay(75);
    }
};
