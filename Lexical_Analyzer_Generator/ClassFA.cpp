#include "ClassFA.h"
#include "recource.h"

FA::FA ()
{
    FA::no_vertices = 2;
    FA::final_state = 1;
    FA::start_state = 0;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : constructs the states of FA
 * Take   : number of states
 *-----------------------------------------------------------------------------------------*/

void FA::set_vertices(int number)
{
    FA::no_vertices = number;
    for(int i=0; i<FA::no_vertices; i++)
    {
        FA::vertices.push_back(i);
    }
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : constructs states of FA
 * Take   : vector of states
 *-----------------------------------------------------------------------------------------*/

void FA::set_vertices (vector<int> vertices_element)
{
    FA::vertices =  vertices_element;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : add final state to the vector that includes all final states of FA
 * Take   : number of final state
 *-----------------------------------------------------------------------------------------*/

void FA::set_final_to_DFA(int state)
{
    FA::final_To_DFA.push_back(state);
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : returning vector that includes all final states
 * RETURN : vector of final states
 *-----------------------------------------------------------------------------------------*/

vector<int> FA::get_final_to_DFA()
{
    return FA::final_To_DFA;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : setting a new transition between two new states in FA, this is used to build graph of FA
 * Take   : two number of states (from-to) and symbol of transition
 *-----------------------------------------------------------------------------------------*/

void FA::set_transtions(int from, int to, char tran_symbol)
{
    struct transition tran;
    tran.vertex_from = from;
    tran.vertex_to = to;
    tran.symbol = tran_symbol;
    FA::transitions.push_back(tran);
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : returning graph of FA
 * RETURN : vector of all transitions in FA
 *-----------------------------------------------------------------------------------------*/

vector<transition> FA::get_tran()
{
    return FA::transitions;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : building graph of FA
 * Take   : vector of all transitions in FA
 *-----------------------------------------------------------------------------------------*/

void FA::set_tran(vector<transition> trans)
{
    FA::transitions = trans;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : returning states of FA
 * RETURN : vector of states
 *-----------------------------------------------------------------------------------------*/

vector<int> FA::get_vertices()
{
    return FA::vertices;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : returning number of states
 * RETURN : number of states
 *-----------------------------------------------------------------------------------------*/

int FA::get_no_vertices()
{
    return FA::no_vertices;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : setting number of states in graph of FA
 * Take   : number of states
 *-----------------------------------------------------------------------------------------*/

void FA::set_no_vertices(int vertices_num)
{
    FA::no_vertices = vertices_num;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : setting number of start state in graph of FA
 * Take   : number of start state in graph
 *-----------------------------------------------------------------------------------------*/

void FA::set_startState(int state)
{
    FA::start_state = state;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : returning number of start state
 * RETURN : number of start state
 *-----------------------------------------------------------------------------------------*/

int FA::get_startState()
{
    return FA::start_state;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : setting number of final state in graph
 * Take   : number of final state
 *-----------------------------------------------------------------------------------------*/

void FA::set_finalState(int state)
{
    FA::final_state = state;
    FA::final_To_DFA.push_back(state);
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : returning number of final state in graph
 * RETURN : number of final state in graph
 *-----------------------------------------------------------------------------------------*/

int FA::get_finalState()
{
    return FA::final_state;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : printing graph of FA as printing transitions of graph
 *-----------------------------------------------------------------------------------------*/

void FA::display()
{
    struct transition new_trans;
    for(int i = 0; i < FA::transitions.size(); i++)
    {
        new_trans = FA::transitions.at(i);
        cout<<"q"<<new_trans.vertex_from<<" --> q"<<new_trans.vertex_to<<" : Symbol is "<<new_trans.symbol<<endl;
    }
    cout<<"\nThe final state is q"<<FA::get_finalState()<<endl;
}
