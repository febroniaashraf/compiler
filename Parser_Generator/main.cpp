#include <iostream>
#include <bits/stdc++.h>
#include <set>
#include <regex>
#include <iterator>
#include<algorithm>
#include "recource.h"
#include "First.h"
#include "Follow.h"
#include "leftMostDerivation.h"
#include "Bonus.h"
#include "Table.h"
using namespace std;


vector<Non_terminal> all_nonTerminals;
vector<string> terminals;
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

void read_inputFile(const char* input_file)
{
    fstream newfile;
    newfile.open(input_file,ios::in); //open a file to perform read operation using file object
    if (newfile.is_open())    //checking whether the file is open
    {
        struct Non_terminal non_terminal;
        terminals.push_back("^");
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
                    non_terminal.max_size = 0;
                    first[word];
                    word="";
                    continue;
                }
                if(line[i]=='#')
                {
                    if(production.size()!=0)
                    {
                        non_terminal.productions.push_back(production);
                        if(production.size() > non_terminal.max_size)
                        {
                            non_terminal.max_size = production.size();
                        }
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
                    if(production.size() > non_terminal.max_size)
                    {
                        non_terminal.max_size = production.size();
                    }
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
            if(production.size() > non_terminal.max_size)
            {
                non_terminal.max_size = production.size();
            }
        }
        all_nonTerminals.push_back(non_terminal);
    }
}


int main()
{
    read_inputFile("input.txt");
    vector<Non_terminal> nonTerminals = all_nonTerminals;
    vector<Non_terminal> left_factor = left_factoring(nonTerminals);
    left_recurion(left_factor);
    construct_transitionDigram();
    print_transitionDigram();
    set_first();
    set_follow();
    map<string, map<string, vector<string>>> m = build_table();
    //print_table(m);
    vector<string> v = leftMostDerivation(m);
    outputFile(v,m);
    return 0;
}
