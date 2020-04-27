#ifndef FIRST_H_INCLUDED
#define FIRST_H_INCLUDED
#include "recource.h"

bool is_terminal (string);
int get_nonTreminal_byName(string);
void get_first(int , int);
void set_first ();
extern vector<string> terminals;
extern map<string, set<string> > first;
extern map<string, set< pair<string, vector<string>>>> firstForTable;

#endif // FIRST_H_INCLUDED
