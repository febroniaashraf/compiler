#ifndef MINIMIZATION_H_INCLUDED
#define MINIMIZATION_H_INCLUDED
#include "recource.h"
#include "ClassFA.h"

vector<int> getNonFinalStates(vector<int> first, vector<int> second);
vector<char> getInputs(vector<transition> transitions);
string findNextStats (int state, vector<transition> transitions, vector<char> inputs,map<int, int> keyOfStates);
void updateMapValues(int value,vector<int> v, std::map<int, int> &keyOfStates);
map<int, vector<int> > minimizaion (FA DFA);
FA  minimizedTable (map<int, vector<int> > partitions,FA DFA);
extern map<int,string> finalStatesMap;
extern map<int,vector<string> > Mini;
#endif // MINIMIZATION_H_INCLUDED
