#ifndef FIRST_H_INCLUDED
#define FIRST_H_INCLUDED
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
int get_nonTreminal_byName(string);
void get_first(int , int );
void set_first ();
extern vector<string> terminals;
extern map<string, set<string> > first;
extern map<string, set< pair<string,vector<string>>> > firstForTable;

#endif // FIRST_H_INCLUDED
