/**
 * File: game_config.h
 * Description: defines constants used in the
 *              tic-tac-toe games
 * 
 * Author: Alexander J. Gaidis (agaidis)
 * E-mail: alexander_gaidis@brown.edu
 */

const int EMPTY = 0;
const int X = 1; /* AI */
const int O = 2; /* Human */
const int NROWS = 3;
const int NCOLS = 3;

const int minimax_depth = 3; /* must be > 0 */

#define iter(i) for(int i = 0; i < NROWS; i++)
#define double_iter(i, j) iter(i) iter(j)

#define other_player(player) ((X == (player)) ? O : X)
