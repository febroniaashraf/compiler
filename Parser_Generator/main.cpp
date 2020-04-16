#include <iostream>
#include <bits/stdc++.h>
#include <set>
#include <iterator>
#include<algorithm>

using namespace std;

struct Non_terminal
{
    string name;
    vector<vector<string> > productions;
};
vector<Non_terminal> all_nonTerminals;
vector<string> terminals;
map<string, set<string> > first;

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

void get_first(int i, int j)
{
    string nonTerminal = all_nonTerminals.at(i).name;
    string str = all_nonTerminals.at(i).productions.at(j).at(0);
    if (is_terminal(str))
    {
        first[nonTerminal].insert(str);
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

void read_inputFile(const char* input_file)
{
    fstream newfile;
    newfile.open(input_file,ios::in); //open a file to perform read operation using file object
    if (newfile.is_open())    //checking whether the file is open
    {
        struct Non_terminal non_terminal;
        string line = "";
        int get = 0;
        vector<string> production;
        while(getline(newfile, line))  //read data from file object and put it into string.
        {
            string word = "";
            bool terminal = false;
            for (std::string::size_type i = 0; i < line.size(); i++)
            {
                if(line[i]=='\\')
                {
                    if(line[i+1]=='L')
                    {
                        word+= '^';
                    }
                    else
                    {
                        word+= line[i+1];
                    }
                    i++;
                    continue;
                }
                if(line[i]==' ' && line[i+1]!='=' && line[i-1] !='=')
                {
                    if(word != "")
                    {
                        production.push_back(word);
                        word = "";
                    }
                    continue;
                }
                if(line[i] == '\'' && terminal)
                {
                    if(word != "")
                    {
                        std::vector<string>::iterator it;
                        it = std::find (terminals.begin(), terminals.end(), word);
                        if (it == terminals.end())
                        {
                            terminals.push_back(word);
                        }
                        production.push_back(word);
                        word = "";
                        terminal = false;
                    }
                    continue;
                }
                if(line[i]=='\'')
                {
                    terminal = true;
                    continue;
                }
                if(line[i]=='=')
                {
                    non_terminal.name = word;
                    first[word];
                    word="";
                    continue;
                }
                if(line[i]=='#')
                {
                    if(production.size()!=0)
                    {
                        non_terminal.productions.push_back(production);
                    }
                    if(get != 0)
                    {
                        all_nonTerminals.push_back(non_terminal);
                    }
                    word = "";
                    non_terminal.name = "";
                    production.clear();
                    non_terminal.productions.clear();
                    get++;
                    continue;
                }
                if(line[i]=='|')
                {
                    non_terminal.productions.push_back(production);
                    production.clear();
                    word = "";
                    continue;
                }
                if(line[i]!=' ' && line[i]!='|' && line[i]!='#' && line[i]!='=')
                {
                    word+= line[i];
                }
            }
            if(word != "")
            {
                production.push_back(word);
                word = "";
            }
        }
        if(production.size()!=0)
        {
            non_terminal.productions.push_back(production);
        }
        all_nonTerminals.push_back(non_terminal);
    }
}


int main()
{
    read_inputFile("input.txt");
    set_first();
    map<string, set<string> >::iterator it1;
    set <string>::iterator it2;
    for(it1 = first.begin(); it1 != first.end(); ++it1)
    {
        std::cout << it1->first << endl;
        set<string> s = it1->second;
        for (it2 = s.begin(); it2 != s.end(); ++it2)
        {
            string str = *it2;
            cout << str << ", ";
        }
        cout << endl;
        cout << "----------------------------" << endl;
    }
    return 0;
}
