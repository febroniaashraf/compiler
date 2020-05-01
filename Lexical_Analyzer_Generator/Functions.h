#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include "recource.h"
#include "ClassFA.h"

FA concatenation (FA a, FA b);
FA kleene(FA a);
FA positive_closure(FA a);
FA Union (vector<FA> all);
FA Union_NFA(vector<FA> all, vector<string> names);
FA range (FA a, FA b);

extern map<int,string> nfa;

#endif // FUNCTIONS_H_INCLUDED
