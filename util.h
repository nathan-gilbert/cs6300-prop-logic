/*
 * Nathan Gilbert
 * Some useful utility functions.
 */
#ifndef UTIL_H
#define UTIL_H

#include "theorem.h"
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

extern int trace;
extern int stats;
extern int debgu;
extern vector<Theorem> kb;

// Prints the CLI options.
void printOptions(void);

// Prints the knowledge base in an easy to read format.
void prettyPrint(void);

// Strings whitespace from strings.
string strip(string, const char *);

// Takes in user input and decides what the next
// action should be.
int parseInput(string);

#endif
