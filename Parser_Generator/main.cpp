#include <iostream>
#include "recource.h"
#include "First.h"
#include "Follow.h"
#include "leftMostDerivation.h"
#include "Bonus.h"
#include "Table.h"
//////////////////////////////////////////////////////////////////////////////////////////////////
//
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

//////////////////////////////////////////////////////////////////////////////////////////////////
//
vector<Non_terminal> all_nonTerminals;
//
vector<string> terminals;
//
vector<pair<string,Transition_Digrams>> tran_Digram;
//////////////////////////////////////////////////////////////////////////////////////////////////

/**----------------------------------------------------------------------------------------
 * USEAGE : This Function reads output file from Phase 1 as an Input for LeftMostDerivation
 * TAKE   : Queue of string contains The lexical analyzer
 *-----------------------------------------------------------------------------------------*/

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

/**----------------------------------------------------------------------------------------
 * USEAGE : This Function reads output file from Phase 1 as an Input for LeftMostDerivation
 * TAKE   : Queue of string contains The lexical analyzer
 *-----------------------------------------------------------------------------------------*/
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

/**----------------------------------------------------------------------------------------
 * USEAGE : This Function reads output file from Phase 1 as an Input for LeftMostDerivation
 * TAKE   : Queue of string contains The lexical analyzer
 *-----------------------------------------------------------------------------------------*/
void print_transitionDigram()
{
    for(int i=0; i<tran_Digram.size(); i++)
    {
        cout << "Name: " <<tran_Digram.at(i).first << endl;
        tran_Digram.at(i).second.display();
    }
}

int main()
{
    read_inputFile("grammar.txt");
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
