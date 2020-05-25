#ifndef CLASSFA_H_INCLUDED
#define CLASSFA_H_INCLUDED
#include "recource.h"

class FA
{
public:
    int no_vertices;
    int final_state;
    int start_state;
    vector<int> final_To_DFA;
    vector<transition> transitions;
    vector<int> vertices;

    FA ();

    void set_vertices(int number);
    void set_vertices (vector<int> vertices_element);
    void set_final_to_DFA(int state);
    vector<int> get_final_to_DFA();
    void set_transtions(int from, int to, string tran_symbol);
    vector<transition> get_tran();
    void set_tran(vector<transition> trans);
    vector<int> get_vertices();
    int get_no_vertices();
    void set_no_vertices(int vertices_num);
    void set_startState(int state);
    int get_startState();
    void set_finalState(int state);
    int get_finalState();
    void display();
};

#endif // CLASSFA_H_INCLUDED
