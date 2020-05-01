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
#include <stack>
using std::stack;
#include <string>
using std::string;
#include <map>
using std::map;

struct transition
{
    int vertex_from;
    int vertex_to;
    char symbol;
};

struct DFAelement
{
    int index;
    vector<int> eq;
    bool mark;
};

#endif // RECOURCE_H_INCLUDED
