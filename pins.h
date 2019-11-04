/**
 * File: pins.h
 * Description: maps arduino pins to game entities
 * 
 * Author: Alexander J. Gaidis (agaidis)
 * E-mail: alexander_gaidis@brown.edu
 */

#include "Arduino.h"

/**
 * The names of the button pins correspond to the row or 
 * column they control like so:
 *
 *     C0:   C1:   C2:
 * R0: B  -  B  -  B
 * R1: B  -  B  -  B
 * R2: B  -  B  -  B
 */
const int buttonR0 = 7;
const int buttonR1 = 5;
const int buttonR2 = 4;

const int buttonC0 = 6;
const int buttonC1 = 3;
const int buttonC2 = 2;

/**
 * The names of the LED pins correspond to the row or 
 * column they control like so:
 *
 *      C0R/C0B:   C1R/C1B:    C2R/C2B:
 *  R0:   LED        LED         LED
 *  R1:   LED        LED         LED
 *  R2:   LED        LED         LED
 *
 * Where C#R controls the red color of the LED and C#B 
 * controls the blue color.
*/
const int ledR0 = A0;
const int ledR1 = A1;
const int ledR2 = A2;

const int ledC0R = 9;
const int ledC0B = 8;
const int ledC1R = 11;
const int ledC1B = 10;
const int ledC2R = 13;
const int ledC2B = 12;
