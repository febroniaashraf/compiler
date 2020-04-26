#include "leftMostDerivation.h"
#include <iostream>
#include <bits/stdc++.h>
#include <regex>
#include "recource.h"

/************************************************************************

*************************************************************************/
queue<string> readOutAsIn()
{
    string line;
    queue<string> queueInputs;
    ifstream myfile ("D:\\codes\\compilerGit\\compiler\\Lexical_Analyzer_Generator\\output.txt");
    regex re("(.*)( --> )(.*)");
    smatch match;
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            if (regex_search(line, match, re) == true)
            {
                if(match.str(3) == "assign")
                {
                    queueInputs.push("=");
                }
                else
                {
                    queueInputs.push(match.str(3));
                }
            }
        }
        myfile.close();
    }
    queueInputs.push("$");
    return queueInputs;
}
/************************************************************************

*************************************************************************/
void leftMostDerivation(map<string, map<string, vector<string> > > table)
{
    std::ofstream outfile;
    outfile.open("output.txt", std::ios_base::trunc); // overwrite
    outfile << "The predictive parsing table : \n\n";
    map<string, map<string, vector<string>>>::iterator it1;
    map<string, vector<string>>::iterator it2;
    for(it1 = table.begin(); it1 != table.end(); ++it1)
    {
        outfile << "Non terminal : "<< it1->first << " --> \n";
        map<string, vector<string>> s = it1->second;
        for (it2 = s.begin(); it2 != s.end(); ++it2)
        {
            string str = it2->first;
            vector<string> str2 = it2->second;
            outfile <<"If "<< str << " then ";
            for(int i=0; i<str2.size(); i++)
            {
                outfile<< str2.at(i)<< " ";
            }

            outfile << ", ";

        }
        outfile << "\n";
        outfile << "----------------------------" << "\n";
    }
    std::queue<std::string> queueInputs = readOutAsIn();
    stack<string> stackProc;
    stackProc.push("$");
    stackProc.push(all_nonTerminals.at(0).name);
    while(queueInputs.front() != "$")
    {
        if(is_terminal(stackProc.top()) && stackProc.top()!="^")
        {
            if(stackProc.top() == queueInputs.front())
            {
                outfile << "accept " + stackProc.top() + "\n";
                stackProc.pop();
                queueInputs.pop();
            }
            else if(stackProc.top() == "$")
            {
                outfile << "error: can't complete" ;
                break;
            }
            else
            {
                outfile<< "error: missing " + stackProc.top() + " inserted" +"\n";
                stackProc.pop();
            }
        }
        else
        {
            if(stackProc.top() == "^")
            {
                stackProc.pop();
            }
            else if(stackProc.top() == "synch")
            {
                outfile << "error3" ;
                stackProc.pop();
            }
            else
            {
                vector<string> ss = table[stackProc.top()][queueInputs.front()];
                if(ss.size() != 0)
                {
                    outfile <<stackProc.top() + " --> ";
                    stackProc.pop();
                    for(int i = ss.size() - 1; i >= 0; i--)
                    {
                        outfile <<ss.at(i) +" ";
                        stackProc.push(ss.at(i));
                    }
                    outfile<<"\n";
                }
                else
                {
                    outfile<<"error: illegal "<< stackProc.top() + " discord "  +queueInputs.front() + "\n";
                    queueInputs.pop();
                }
            }
        }
    }
    while(stackProc.top() != "$")
    {
        if(is_terminal(stackProc.top()))
        {
            outfile <<stackProc.top()+ " missing " +"\n";
        }
        stackProc.pop();
    }

    outfile.close();

}
