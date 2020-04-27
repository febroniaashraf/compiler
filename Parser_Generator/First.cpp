#include "First.h"
#include "recource.h"
map<string, set<string> > first;
bool is_terminal (string str)
{
    std::vector<string>::iterator it;
    it = std::find(terminals.begin(), terminals.end(),str);
    if (it != terminals.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}
int get_nonTreminal_byName(string str)
{
    for(int i=0; i<all_nonTerminals.size(); i++)
    {
        if (all_nonTerminals.at(i).name == str)
        {
            return i;
        }
    }
}
void get_first(int i, int j)
{
    string nonTerminal = all_nonTerminals.at(i).name;
    string str = all_nonTerminals.at(i).productions.at(j).at(0);
    if (is_terminal(str))
    {
        first[nonTerminal].insert(str);
        pair<string,vector<string>> x = make_pair(str, all_nonTerminals.at(i).productions.at(j));
        firstForTable[nonTerminal].insert(x);
        return;
    }
    int newIndex = get_nonTreminal_byName(str);
    nonTerminal = all_nonTerminals.at(newIndex).name;
    for (int k = 0 ;  k < all_nonTerminals.at(newIndex).productions.size(); k++)
    {
        get_first(newIndex,k);
    }
    set <string>::iterator it2;
    set<string> s = first[str];
    for(it2 = s.begin(); it2 != s.end(); ++it2)
    {
        first[all_nonTerminals.at(i).name].insert(*it2);
    }
    set <pair<string,vector<string>>>::iterator it;
    set<pair<string,vector<string>>> s1 = firstForTable[str];
    for(it = s1.begin(); it != s1.end(); ++it)
    {
        pair<string,vector<string>> p ;
        p.first = (*it).first;
        p.second = all_nonTerminals.at(i).productions.at(j);
        firstForTable[all_nonTerminals.at(i).name].insert(p);
    }
}

void set_first ()
{
    for(int i = 0; i < all_nonTerminals.size(); i++)
    {
        for(int j=0; j<all_nonTerminals.at(i).productions.size(); j++)
        {
            string str = all_nonTerminals.at(i).productions.at(j).at(0);
            get_first(i,j);
        }
    }
}
