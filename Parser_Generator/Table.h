#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED
#include "recource.h"

map<string, map<string, vector<string>>> build_table();
extern map<string, set<string> > follow;
extern map<string, set< pair<string,vector<string>>> > firstForTable;

#endif // TABLE_H_INCLUDED
