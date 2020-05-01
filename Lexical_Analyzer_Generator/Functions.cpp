#include "Functions.h"
#include "recource.h"
#include "ClassFA.h"

map<int,string> nfa;

/**-----------------------------------------------------------------------------------------
 * USEAGE : Concatenate two FA
 * Take   : two Finite Automata
 * RETURN : concatenation of the two FA
 *-----------------------------------------------------------------------------------------*/

FA concatenation (FA a, FA b)
{
    FA result;
    result.set_vertices((a.get_no_vertices() + b.get_no_vertices()));
    result.set_startState(a.get_startState());
    result.set_finalState(a.get_no_vertices() + b.get_no_vertices()-1);
    for (int i = 0 ; i < a.get_tran().size(); i++)
    {
        struct transition tran;
        tran = a.get_tran().at(i);
        result.set_transtions(tran.vertex_from,tran.vertex_to,tran.symbol);
    }
    result.set_transtions(a.get_finalState(),b.get_startState()+a.get_no_vertices(),'^');
    for (int i = 0 ; i < b.get_tran().size(); i++)
    {
        struct transition tran;
        tran = b.get_tran().at(i);
        result.set_transtions(tran.vertex_from+a.get_no_vertices(),tran.vertex_to+a.get_no_vertices(),tran.symbol);
    }
    return result;

}

/**-----------------------------------------------------------------------------------------
 * USEAGE : Applying kleene closure to FA
 * Take   : Finite Automata
 * RETURN : result of kleene closure
 *-----------------------------------------------------------------------------------------*/

FA kleene(FA a)
{
    FA result;
    result.set_vertices(a.get_no_vertices() + 2);
    result.set_startState(a.get_startState());
    result.set_finalState(a.get_finalState() + 2);
    result.set_transtions(result.get_startState(), a.get_startState() + 1, '^');
    result.set_transtions(result.get_startState(), result.get_finalState(), '^');
    for (int i = 0 ; i < a.get_tran().size(); i++)
    {
        struct transition tran;
        tran = a.get_tran().at(i);
        result.set_transtions(tran.vertex_from+1,tran.vertex_to+1,tran.symbol);
    }
    result.set_transtions(a.get_finalState()+1,result.get_finalState(),'^');
    result.set_transtions(result.get_finalState()- 1,result.get_startState() + 1,'^');
    return result;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : Applying positive closure to FA
 * Take   : Finite Automata
 * RETURN : result of positive closure
 *-----------------------------------------------------------------------------------------*/

FA positive_closure(FA a)
{
    FA result;
    result.set_vertices(a.get_no_vertices()+2);
    result.set_startState(a.get_startState());
    result.set_finalState(a.get_finalState()+2);
    result.set_transtions(result.get_startState(),a.get_startState()+1,'^');
    for(int i=0; i<a.get_tran().size(); i++)
    {
        struct transition tran;
        tran = a.get_tran().at(i);
        result.set_transtions(tran.vertex_from+1,tran.vertex_to+1,tran.symbol);
    }
    result.set_transtions(a.get_finalState()+1,result.get_finalState(),'^');
    result.set_transtions(result.get_finalState()-1,result.get_startState()+1,'^');
    return result;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : Union two or more Finite Automata with each other
 * Take   : vector of Finite Automata
 * RETURN : result of union operation
 *-----------------------------------------------------------------------------------------*/

FA Union (vector<FA> all)
{
    FA result;
    int sum = 0;
    for(int i=0; i<all.size(); i++)
    {
        sum = sum+ all.at(i).get_no_vertices();
    }
    result.set_vertices(sum+2);
    result.set_startState(0);
    result.set_finalState(sum+1);
    for(int i=0; i<all.size(); i++)
    {
        int start = 0;
        for(int j=0; j<i; j++)
        {
            start = start+all.at(j).get_no_vertices();
        }
        result.set_transtions(result.get_startState(), all.at(i).get_startState()+start+1, '^');
        for(int k = 0; k<all.at(i).get_tran().size(); k++)
        {
            struct transition tran;
            tran = all.at(i).get_tran().at(k);
            result.set_transtions(tran.vertex_from+start+1,tran.vertex_to+start+1,tran.symbol);
        }
        result.set_transtions(all.at(i).get_finalState()+start+1,result.get_finalState(),'^');
    }
    return result;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : Union two or more NFA together
 * Take   : vector of NFA and vector of their name
 * RETURN : result of union operation which is final NFA
 *-----------------------------------------------------------------------------------------*/

FA Union_NFA(vector<FA> all, vector<string> names)
{
    FA result;
    int sum = 0;
    for(int i=0; i<all.size(); i++)
    {
        sum = sum+ all.at(i).get_no_vertices();
    }
    result.set_vertices(sum+1);
    result.set_startState(0);
    for(int i=0; i<all.size(); i++)
    {
        int start = 0;
        for(int j=0; j<i; j++)
        {
            start = start+all.at(j).get_no_vertices();
        }
        result.set_transtions(result.get_startState(), all.at(i).get_startState()+start+1, '^');
        for(int k = 0; k<all.at(i).get_tran().size(); k++)
        {
            struct transition tran;
            tran = all.at(i).get_tran().at(k);
            result.set_transtions(tran.vertex_from+start+1,tran.vertex_to+start+1,tran.symbol);
        }
        result.set_final_to_DFA(all.at(i).get_finalState()+start+1);
        nfa.insert(pair<int,string>(all.at(i).get_finalState()+start+1, names.at(i)));
    }
    return result;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : getting NFA to two FA as they have - (range) between them
 * Take   : two Finite Automata
 * RETURN : result of range operation which is FA
 *-----------------------------------------------------------------------------------------*/

FA range (FA a, FA b)
{
    char first = a.transitions[0].symbol;
    char last = b.transitions[0].symbol;
    vector<char> v_char;
    vector<FA> v_fa;
    for (char i = first ; i <= last ; i++)
    {
        v_char.push_back(i);
    }

    for(int i=0; i< v_char.size(); i++)
    {
        FA sym;
        sym.set_vertices(2);
        sym.set_startState(0);
        sym.set_finalState(1);
        sym.set_transtions(sym.get_startState(),sym.get_finalState(),v_char[i]);
        v_fa.push_back(sym);
    }
    FA result;
    vector<FA> all;
    for (int i = 0 ; i < v_fa.size() ; i++)
    {
        all.push_back(v_fa[i]);
    }
    result = Union (all);
    return result;
}
