#ifndef LEFTMOSTDERIVATION_H_INCLUDED
#define LEFTMOSTDERIVATION_H_INCLUDED
#include "recource.h"
#include <vector>
using std::vector;
#include <queue>
using std::queue;
#include <string>
using std::string;
#include <map>
using std::map;
queue<string> readOutAsIn();
void leftMostDerivation(map<string, map<string, vector<string> > > );
extern vector<Non_terminal> all_nonTerminals;

#endif // LEFTMOSTDERIVATION_H_INCLUDED
