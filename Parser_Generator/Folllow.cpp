#include "Follow.h"
#include "First.h"
#include "recource.h"
///////////////////////////////////////////////////////////////////////////////////////////
// map contains the follow set
map<string, set<string> > follow;
///////////////////////////////////////////////////////////////////////////////////////////

/**-----------------------------------------------------------------------------------------
 * USEAGE : Build the follow set of each nonterminal
 * Take   : production to each nonterminal
 *-----------------------------------------------------------------------------------------*/

void get_follow(string name)
{
    for(int i=0; i<all_nonTerminals.size(); i++)
    {
        for(int j=0; j<all_nonTerminals.at(i).productions.size(); j++)
        {
            vector<string> prod = all_nonTerminals.at(i).productions.at(j);
            bool flag = false;
            for(int k = 0; k<prod.size(); k++)
            {
                if(prod.at(k) == name)
                {
                    if(k!= prod.size()-1 && is_terminal(prod.at(k+1)))
                    {
                        follow[name].insert(prod.at(k+1));
                        break;
                    }
                    else if(k!= prod.size()-1 && !is_terminal(prod.at(k+1)))
                    {
                        set<string> first_of_next = first[prod.at(k+1)];
                        set <string>::iterator it2;
                        for(it2 = first_of_next.begin(); it2 != first_of_next.end(); ++it2)
                        {
                            if(*it2 != "^")
                            {
                                follow[name].insert(*it2);
                            }
                            else
                            {
                                flag = true;
                            }
                        }
                        if(!flag)
                        {
                            break;
                        }
                        else
                        {
                            if(k!=prod.size()-2)
                            {
                                flag = false;
                            }
                        }
                    }
                    else
                    {
                        if(name != all_nonTerminals.at(i).name)
                        {
                            get_follow(all_nonTerminals.at(i).name);
                            set<string> follows = follow[all_nonTerminals.at(i).name];
                            set <string>::iterator it;
                            for(it = follows.begin(); it != follows.end(); ++it)
                            {
                                follow[name].insert(*it);
                            }
                        }
                    }
                    if(flag)
                    {
                        if(name != all_nonTerminals.at(i).name)
                        {
                            get_follow(all_nonTerminals.at(i).name);
                            set<string> follows = follow[all_nonTerminals.at(i).name];
                            set <string>::iterator it;
                            for(it = follows.begin(); it != follows.end(); ++it)
                            {
                                follow[name].insert(*it);
                            }
                        }
                    }
                }
            }
        }
    }
}

/**----------------------------------------------------------------------------------------
 * USEAGE :  build the follow set of each nonterminal as it calls the function get_follow
 *----------------------------------------------------------------------------------------*/

void set_follow()
{
    for(int i=0; i<all_nonTerminals.size(); i++)
    {
        string name = all_nonTerminals.at(i).name;
        if(i==0)
        {
            follow[name].insert("$");
        }
        get_follow(name);
    }
}
