#ifndef FOLLOW_H_INCLUDED
#define FOLLOW_H_INCLUDED
#include "recource.h"

void get_follow(string);
void set_follow();
extern map<string, set<string> > follow;
extern map<string, set<string> > first;

#endif // FOLLOW_H_INCLUDED
