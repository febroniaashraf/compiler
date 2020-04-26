#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED
#include <iostream>
using namespace std;
#include <vector>
using std::vector;
#include <queue>
using std::queue;
#include <string>
using std::string;
#include <map>
using std::map;
#include <set>
using std::set;
map<string, map<string, vector<string>>> build_table();
extern map<string, set<string> > follow;
extern map<string, set< pair<string,vector<string>>> > firstForTable;

#endif // TABLE_H_INCLUDED
