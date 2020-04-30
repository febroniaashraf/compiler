#ifndef RECOURCE_H_INCLUDED
#define RECOURCE_H_INCLUDED

#include <iostream>
#include <bits/stdc++.h>
#include <set>
#include <regex>
#include <iterator>
#include<algorithm>

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

struct transition
{
    int vertex_from;
    int vertex_to;
    string symbol;
};

struct Non_terminal
{
    string name;
    vector<vector<string>> productions;
    int max_size;
};

extern vector<Non_terminal> all_nonTerminals;
extern vector<string> terminals;


#endif // RECOURCE_H_INCLUDED
