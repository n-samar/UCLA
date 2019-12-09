#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>


///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;            // max number of rows in the arena
const int MAXCOLS = 20;            // max number of columns in the arena
const int MAXRATS = 100;           // max number of rats allowed
const int INITIAL_RAT_HEALTH = 2;  // initial rat health
const int POISONED_IDLE_TIME = 1;  // poisoned rat idles this many turns
                                   //   between moves

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY      = 0;
const int HAS_POISON = 1;

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int min, int max);
bool decodeDirection(char ch, int& dir);

void clearScreen();

#endif