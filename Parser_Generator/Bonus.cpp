#include "recource.h"
#include "Bonus.h"

class Transition_Digrams
{
public:
    int no_vertices;
    int final_state;
    int start_state;
    vector<transition> transitions;
    vector<int> vertices;

    Transition_Digrams ()
    {
        no_vertices = 2;
        final_state = 1;
        start_state = 0;
    }
    void set_transtions(int from, int to, string tran_symbol)
    {
        struct transition tran;
        tran.vertex_from = from;
        tran.vertex_to = to;
        tran.symbol = tran_symbol;
        transitions.push_back(tran);
    }

    vector<transition> get_tran()
    {
        return transitions;
    }

    void set_startState(int state)
    {
        start_state = state;
    }

    int get_startState()
    {
        return start_state;
    }

    void set_finalState(int state)
    {
        final_state = state;
    }

    int get_finalState()
    {
        return final_state;
    }
    void display()
    {
        struct transition new_trans;
        for(int i = 0; i < transitions.size(); i++)
        {
            new_trans = transitions.at(i);
            cout<<"q"<<new_trans.vertex_from<<" --> q"<<new_trans.vertex_to<<" : Symbol is "<<new_trans.symbol<<endl;
        }
        cout<<"\nThe final state is q"<<get_finalState()<<endl;
    }
};
vector<pair<string,Transition_Digrams>> tran_Digram;
vector<Non_terminal> left_factoring(vector<Non_terminal> input)
{
    vector<Non_terminal> leftFactoring; // to store the left-factored grammar.
    map<string, set< vector<string> > > modified; // to store modified parts of the productions.
    map<string, set< vector<string> > > dash_prods; // to store the new produductions.
    char dash = '`';
    for(int i=0; i<input.size(); i++)
    {
        string name = input.at(i).name;
        int counter = 0;
        for(int j1=0; j1<input.at(i).productions.size(); j1++)
        {
            for(int j2=j1 + 1; j2 < input.at(i).productions.size(); j2++)
            {
                vector <string> prod1 = input.at(i).productions.at(j1);
                vector <string> prod2 = input.at(i).productions.at(j2);
                vector <string> common; // to store the common prefix if exists.
                string newNonTerminal; // a new non terminal for the new production.
                int pos = 0;
                for(int k=0; k<prod1.size() && k<prod2.size(); k++)
                {
                    //check if there is a common prefix.
                    if(prod1[k] == prod2[k])
                    {
                        common.push_back(prod1[k]);
                        pos = k + 1; // the start position of the rest of the productions after the common prefix.
                    }
                    else
                    {
                        break;
                    }
                }
                if (!common.empty())
                {
                    int n = 0;
                    bool flag = false;
                    vector <string> epsilon, s1, s2; // to store the rest of the productions after the common prefix.
                    map<string, set< vector<string> > >::iterator it1;
                    it1 = modified.find(name);
                    // check if the common prefix already exists in the modified map to avoid creating a new non terminal for the same common prefix.
                    if(!(it1 == modified.end()))
                    {
                        set< vector<string> > m = it1->second;
                        set< vector<string> >::iterator it2;
                        for (it2 = m.begin(); it2 != m.end(); ++it2)
                        {
                            vector<string> v = *it2;
                            int counter = 0;
                            for (int i = 0; i < common.size() ; i++)
                            {
                                if (common[i] == v[i])
                                {
                                    if ( i == common.size() - 1)
                                    {
                                        newNonTerminal = v[i+1];
                                        flag = true;
                                    }
                                    else
                                    {
                                        continue;
                                    }
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                    }
                    // create a new non terminal if there is no a non terminal for the same common prefix.
                    if(!flag)
                    {
                        newNonTerminal = name + dash;
                        while(n < modified[name].size())
                        {
                            newNonTerminal += dash;
                            n++;
                        }
                        common.push_back(newNonTerminal);
                        modified[name].insert(common);
                    }
                    dash_prods[newNonTerminal];
                    if (pos == prod1.size()|| pos == prod2.size())
                    {
                        epsilon.push_back("^");
                        dash_prods[newNonTerminal].insert(epsilon);
                    }
                    for(int i1 = pos ; i1 < prod1.size() ; i1++)
                    {
                        s1.push_back(prod1[i1]);
                    }
                    if(!s1.empty())
                    {
                        dash_prods[newNonTerminal].insert(s1);
                    }
                    for(int i2 = pos ; i2 < prod2.size() ; i2++)
                    {
                        s2.push_back(prod2[i2]);
                    }
                    if(!s2.empty())
                    {
                        dash_prods[newNonTerminal].insert(s2);
                    }
                }
            }
        }
    }
    for(int i=0; i<input.size(); i++)
    {
        string name = input.at(i).name;
        Non_terminal nt;
        nt.name = name;
        map<string, set< vector<string> > >::iterator it1;
        it1 = modified.find(name);
        if(it1 == modified.end())
        {
            // it is a non modified rule, so simply add it to the left-factored grammar.
            nt.productions = input.at(i).productions;
            nt.max_size = input.at(i).max_size;
            leftFactoring.push_back(nt);
        }
        else
        {
            // it is a modified rule.
            set< vector<string> > m = it1->second;
            set< vector<string> >::iterator it2;
            nt.max_size = 0;
            for (it2 = m.begin(); it2 != m.end(); ++it2)
            {
                nt.productions.push_back(*it2); // add the modified part.
                vector<string> v = *it2;
                if(v.size() > nt.max_size)
                {
                    nt.max_size = v.size();
                }
            }
            for(int k=0; k<input.at(i).productions.size(); k++)
            {
                string str1 = input.at(i).productions.at(k).at(0);
                bool flag = false;
                for (it2 = m.begin(); it2 != m.end(); ++it2)
                {
                    vector<string> v = *it2;
                    string str2 = v.at(0);
                    if (str1 == str2)
                    {
                        flag = true;
                        break;
                    }
                }
                if(!flag)
                {
                    nt.productions.push_back(input.at(i).productions.at(k)); // add the non modified parts.
                    if(input.at(i).productions.at(k).size() > nt.max_size)
                    {
                        nt.max_size = input.at(i).productions.at(k).size();
                    }
                }
            }
            leftFactoring.push_back(nt);
        }
    }
    // add the new productions to the left-factored grammar.
    map<string, set< vector<string> > >::iterator it;
    for ( it = dash_prods.begin(); it != dash_prods.end(); it++ )
    {
        Non_terminal nt;
        nt.name = it->first;
        nt.max_size = 0;
        set< vector<string> > m = it->second;
        set< vector<string> >::iterator it2;
        for (it2 = m.begin(); it2 != m.end(); ++it2)
        {
            nt.productions.push_back(*it2);
            vector<string> v = *it2;
            if(v.size() > nt.max_size)
            {
                nt.max_size = v.size();
            }
        }
        leftFactoring.push_back(nt);
    }
    return leftFactoring;
}

void left_recurion(vector<Non_terminal> leftFactoring)
{
    vector<Non_terminal> final_nonTerminals;
    for(int i=0; i<leftFactoring.size(); i++)
    {
        string name = leftFactoring.at(i).name;
        bool found = false;
        for(int j=0; j<leftFactoring.at(i).productions.size(); j++)
        {
            string prod = leftFactoring.at(i).productions.at(j).at(0);
            if(prod == name)
            {
                found = true;
                vector<string> remove_production = leftFactoring.at(i).productions.at(j);
                struct Non_terminal new_NonTerminal, current_NonTerminal;
                current_NonTerminal.max_size = 0;
                new_NonTerminal.max_size = 0;
                current_NonTerminal.name = name;
                new_NonTerminal.name = name+"_dash";
                vector<string> current_production;
                if(leftFactoring.at(i).productions.size()==1)
                {
                    current_production.push_back(new_NonTerminal.name);
                    current_NonTerminal.productions.push_back(current_production);
                }
                for(int k=0; k<leftFactoring.at(i).productions.size(); k++)
                {
                    if(j!=k)
                    {
                        current_production = leftFactoring.at(i).productions.at(k);
                        current_production.push_back(new_NonTerminal.name);
                        current_NonTerminal.productions.push_back(current_production);
                        if(current_production.size()>current_NonTerminal.max_size)
                        {
                            current_NonTerminal.max_size = current_production.size();
                        }
                    }

                }
                final_nonTerminals.push_back(current_NonTerminal);
                remove_production.erase(remove_production.begin());
                remove_production.push_back(new_NonTerminal.name);
                vector<string> epsilon;
                epsilon.push_back("^");
                new_NonTerminal.productions.push_back(remove_production);
                new_NonTerminal.productions.push_back(epsilon);
                new_NonTerminal.max_size = remove_production.size();
                final_nonTerminals.push_back(new_NonTerminal);
                break;
            }
        }
        if(!found)
        {
            final_nonTerminals.push_back(leftFactoring.at(i));
        }
    }
    all_nonTerminals.clear();
    all_nonTerminals = final_nonTerminals;
}
void construct_transitionDigram()
{
    for(int i=0; i<all_nonTerminals.size(); i++)
    {
        Transition_Digrams tran;
        struct Non_terminal nonTerminal = all_nonTerminals.at(i);
        if(i==0)
        {
            tran.set_startState(0);
            tran.set_finalState(nonTerminal.max_size);
        }
        else
        {
            tran.set_startState(tran_Digram.at(i-1).second.get_finalState()+1);
            tran.set_finalState(nonTerminal.max_size+tran_Digram.at(i-1).second.get_finalState()+1);
        }
        for(int j=0; j<nonTerminal.productions.size(); j++)
        {
            for(int k=0; k<nonTerminal.productions.at(j).size(); k++)
            {
                if(nonTerminal.productions.at(j).size()==1)
                {
                    tran.set_transtions(tran.get_startState(),tran.get_finalState(),nonTerminal.productions.at(j).at(k));
                }
                else if(k==0)
                {
                    if(i==0)
                    {
                        tran.set_transtions(tran.get_startState(),k+1,nonTerminal.productions.at(j).at(k));
                    }
                    else
                    {
                        tran.set_transtions(tran.get_startState(),k+tran_Digram.at(i-1).second.get_finalState()+2,nonTerminal.productions.at(j).at(k));
                    }
                }
                else if(k==nonTerminal.productions.at(j).size()-1)
                {
                    if(i==0)
                    {
                        tran.set_transtions(k,tran.get_finalState(),nonTerminal.productions.at(j).at(k));
                    }
                    else
                    {
                        tran.set_transtions(k+tran_Digram.at(i-1).second.get_finalState()+1,tran.get_finalState(),nonTerminal.productions.at(j).at(k));
                    }
                }
                else
                {
                    if(i==0)
                    {
                        tran.set_transtions(k,k+1,nonTerminal.productions.at(j).at(k));
                    }
                    else
                    {
                        tran.set_transtions(k+tran_Digram.at(i-1).second.get_finalState()+1,k+tran_Digram.at(i-1).second.get_finalState()+2,nonTerminal.productions.at(j).at(k));
                    }
                }
            }
        }
        pair<string,Transition_Digrams> digram;
        digram.first = nonTerminal.name;
        digram.second = tran;
        tran_Digram.push_back(digram);
    }
}


void print_transitionDigram()
{
    for(int i=0; i<tran_Digram.size(); i++)
    {
        cout << "Name: " <<tran_Digram.at(i).first << endl;
        tran_Digram.at(i).second.display();
    }
}
