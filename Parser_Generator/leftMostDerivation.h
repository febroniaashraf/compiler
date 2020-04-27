#ifndef LEFTMOSTDERIVATION_H_INCLUDED
#define LEFTMOSTDERIVATION_H_INCLUDED
#include "recource.h"

queue<string> readOutAsIn();
vector<string> leftMostDerivation(map<string, map<string, vector<string> > > );
extern vector<Non_terminal> all_nonTerminals;
void outputFile(vector<string> ,map<string, map<string, vector<string> > > );

#endif // LEFTMOSTDERIVATION_H_INCLUDED
