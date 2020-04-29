#ifndef DFA_H_INCLUDED
#define DFA_H_INCLUDED
#include "recource.h"
#include "ClassFA.h"

FA NFAtoDFA (FA a);

extern int dummyState;
extern map<int,string> nfa;
extern map<int,string> finalStatesMap;
#endif // DFA_H_INCLUDED
