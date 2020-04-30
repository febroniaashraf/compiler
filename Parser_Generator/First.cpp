#include "First.h"
#include "recource.h"
map<string, set<string> > first;
bool is_terminal (string str) // check if a string is a terminal or not.
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
int get_nonTreminal_byName(string str) // get the index of a certain non terminal by its name.
{
    for(int i=0; i<all_nonTerminals.size(); i++)
    {
        if (all_nonTerminals.at(i).name == str)
        {
            return i;
        }
    }
}
void get_first(int i, int j) // a recursive function.
{
    // i is the index of a non terminal in all_nonTerminals , j is the index of its first production.
    string nonTerminal = all_nonTerminals.at(i).name;
    string str = all_nonTerminals.at(i).productions.at(j).at(0);
    // base case.
    if (is_terminal(str))
    {
        first[nonTerminal].insert(str);
        pair<string,vector<string>> x = make_pair(str, all_nonTerminals.at(i).productions.at(j));
        firstForTable[nonTerminal].insert(x);
        return;
    }
    // it is a non terminal.
    int newIndex = get_nonTreminal_byName(str); // get the index of the non terminal.
    nonTerminal = all_nonTerminals.at(newIndex).name;
    for (int k = 0 ;  k < all_nonTerminals.at(newIndex).productions.size(); k++)
    {
        get_first(newIndex,k); // recursion.
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

void set_first () // iterate over all_nonTerminals to get the first set for each non terminal.
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
