#include "DFA.h"
#include "recource.h"
#include "ClassFA.h"

map<int,string> finalStatesMap;
int dummyState;

/**-----------------------------------------------------------------------------------------
 * USEAGE : convert NFA to DFA
 * Take   : NFA
 * RETURN : DFA
 *-----------------------------------------------------------------------------------------*/

FA NFAtoDFA (FA a)
{
    FA DFA;
    vector<transition> transitions = a.get_tran();
    vector<int> finalStates = a.get_final_to_DFA();
    vector<char> symbols;
    DFA.set_startState(0);
    for(int i=0; i<transitions.size(); i++) //GET ALL SYMBOLS IN NFA
    {
        char s = transitions.at(i).symbol;
        if(s != '^'){
        bool contain= false;
        for(int j=0; j<symbols.size(); j++)
        {
            if(symbols.at(j) == s)
            {
                contain=true;
                break;
            }
        }
        if(!contain)
        {
            symbols.push_back(s);
        }
        }
    }

    vector<DFAelement> elements;
    struct DFAelement elem;
    elem.index =0;
    elem.mark= false;
    elem.eq.push_back(a.get_startState());
    int size_elem =0;
    while(size_elem < elem.eq.size())  //get S0 closure
    {
        int j = elem.eq.at(size_elem);
        for(int i =0; i< transitions.size(); i++)
        {
            struct transition t = transitions.at(i);
            if(t.vertex_from == j && t.symbol == '^')
            {
                bool inS0 = false;
                for(int inS=0; inS<elem.eq.size(); inS++)
                {
                    if(elem.eq.at(inS) ==t.vertex_to )
                    {
                        inS0 = true;
                        break;
                    }
                }
                if(!inS0)
                {
                    elem.eq.push_back(t.vertex_to);

                }
            }
        }
        size_elem ++;
    }
    elements.push_back(elem);
    int in=0;
    int increase=0;
    while(in<elements.size()) //CALCULATE THE NEW STATES
    {
        struct DFAelement e = elements.at(in);

        if(!e.mark) //CHECK IF WE TAKE IT BEFORE OR NOT
        {
            e.mark= true;
            e.index=in;

            for(int sym=0; sym<symbols.size(); sym++) //GET THE NEXT STATE FOR EVERY SYMBOL
            {
                char currentSymbol = symbols.at(sym);
                if(currentSymbol != '^')
                {
                    struct DFAelement eNew;
                    eNew.mark=false;
                    for(int j=0; j<e.eq.size(); j++)
                    {
                        int from = e.eq.at(j);
                        bool inE = false;
                        for(int k =0; k< transitions.size(); k++)
                        {
                            struct transition t = transitions.at(k);
                            if(t.vertex_from == from && t.symbol == currentSymbol)
                            {
                                for(int s=0; s<eNew.eq.size(); s++)
                                {
                                    if(eNew.eq.at(s) == t.vertex_to)
                                    {
                                        inE=true;
                                        break;
                                    }
                                }
                                if(!inE)
                                {
                                    eNew.eq.push_back(t.vertex_to);
                                }
                            }
                        }
                    }

                    int j =0;
                    while(j < eNew.eq.size())  //get S closure
                    {
                        int from = eNew.eq.at(j);
                        for(int k =0; k< transitions.size(); k++)
                        {
                            struct transition t = transitions.at(k);
                            if(t.vertex_from == from && t.symbol == '^')
                            {
                                bool inE = false;
                                for(int s=0; s<eNew.eq.size(); s++)
                                {
                                    if(eNew.eq.at(s) == t.vertex_to)
                                    {
                                        inE=true;
                                        break;
                                    }
                                }
                                if(!inE)
                                {
                                    eNew.eq.push_back(t.vertex_to);
                                }
                            }
                        }
                        j ++;
                    }

                    bool inElements = false;
                    int indexNew=0;
                    for(int elem=0; elem<elements.size(); elem++) //CHECK IF THIS STATE ADDED BEFORE TO ELEMENTS OR NOT
                    {
                        struct DFAelement d = elements.at(elem);
                        if(d.eq.size() == eNew.eq.size())
                        {
                            int tempSize=0;
                            for(int temp1=0; temp1<d.eq.size(); temp1++)
                            {
                                for(int temp2=0; temp2<eNew.eq.size(); temp2++)
                                {
                                    if(d.eq.at(temp1) == eNew.eq.at(temp2))
                                    {
                                        tempSize++;
                                    }
                                }
                            }
                            if(tempSize == d.eq.size())
                            {
                                inElements = true;
                                indexNew= d.index;
                                break;
                            }
                        }
                    }
                    if(!inElements)  //if it is a new state then push it to elements
                    {
                        increase++;
                        eNew.index=increase;
                        elements.push_back(eNew);
                        if(eNew.eq.size()==0)
                        {
                            dummyState = increase;
                        }
                        bool fin = false; //check if this state is a final state or not
                        string accept;
                        int finalS=0;
                        for(finalS=0; finalS<eNew.eq.size(); finalS++){
                            if(nfa.count(eNew.eq.at(finalS))){
                                    fin = true;
                                    accept = nfa[eNew.eq.at(finalS)];
                                    break;
                            }
                        }
                        if(fin)
                        {
                            DFA.set_final_to_DFA(eNew.index);
                            finalStatesMap.insert(pair<int,string>(eNew.index,accept));
                        }
                    }
                    else
                    {
                        eNew.index=indexNew;
                    }
                    DFA.set_transtions(in,eNew.index,currentSymbol);
                }
            }
        }
        in++;
    }
    DFA.set_vertices(elements.size());
    return DFA;
}
