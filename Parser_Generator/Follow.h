#ifndef FOLLOW_H_INCLUDED
#define FOLLOW_H_INCLUDED
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
void get_follow(string);
void set_follow();
extern map<string, set<string> > follow;
extern map<string, set<string> > first;

#endif // FOLLOW_H_INCLUDED
