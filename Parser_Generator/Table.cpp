#include "Table.h"
#include "recource.h"
#include "First.h"

map<string, set<pair<string,vector<string>>> > firstForTable;

map<string, map<string, vector<string>>> build_table()
{

    map<string, map<string, vector<string>>> table;
    map<string, set<pair<string,vector<string>>> >::iterator TerminalIterator;
    set <pair<string,vector<string>>>::iterator FirstIterator;
    for(TerminalIterator = firstForTable.begin(); TerminalIterator != firstForTable.end(); ++TerminalIterator)
    {
        string terminal = TerminalIterator->first;
        set<pair<string,vector<string>>> firstSet = TerminalIterator->second;
        for (FirstIterator = firstSet.begin(); FirstIterator != firstSet.end(); ++FirstIterator)
        {
            pair<string,vector<string>> p = *FirstIterator;
            if(p.first != "^")
            {
                table[terminal][p.first] = p.second;
            }
        }
    }

    map<string, set<string> >::iterator TerminalIterator2;
    set <string>::iterator followIterator;
    for(TerminalIterator2 = follow.begin(); TerminalIterator2 != follow.end(); ++TerminalIterator2)
    {
        string terminal = TerminalIterator2->first;
        set<string> followSet = TerminalIterator2->second;
        bool epsilon= false;
        int n = get_nonTreminal_byName(terminal);
        struct Non_terminal non = all_nonTerminals.at(n);
        for(int i=0; i< non.productions.size(); i++)
        {
            if(non.productions.at(i).at(0) == "^")
            {
                epsilon = true;
                break;
            }
        }
        vector<string> v ;
        if(epsilon)
        {
            v.push_back("^");
        }
        else
        {
            v.push_back("synch");
        }
        for (followIterator = followSet.begin(); followIterator != followSet.end(); ++followIterator)
        {
            string str = *followIterator;
            if(table[terminal][str].size()==0)
            {
                table[terminal][str] = v;
            }

        }

    }
    return table;
}
