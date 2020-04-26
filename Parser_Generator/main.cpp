#include <iostream>
#include <bits/stdc++.h>
#include <set>
#include <regex>
#include <iterator>
#include<algorithm>

using namespace std;
typedef pair<string,vector<string>> pairs;
struct transition
{
    int vertex_from;
    int vertex_to;
    string symbol;
};
struct Non_terminal
{
    string name;
    vector<vector<string> > productions;
    int max_size;
};

vector<Non_terminal> all_nonTerminals;
vector<string> terminals;
map<string, set<string> > first;
map<string, set<string> > follow;
map<string, set<pairs> > firstForTable;
map<string,vector<string> > terminalsColumn;

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

    void set_vertices (vector<int> vertices_element)
    {
        vertices =  vertices_element;
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

    vector<int> get_vertices()
    {
        return vertices;
    }

    int get_no_vertices()
    {
        return no_vertices;
    }
    void set_no_vertices(int vertices_num)
    {
        no_vertices = vertices_num;
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

vector<Non_terminal> left_factoring(vector<Non_terminal> input)
{
    vector<Non_terminal> leftFactoring;
    map<string, set< vector<string> > > modified;
    map<string, set< vector<string> > > dash_prods;
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
                vector <string> common;
                string newNonTerminal;
                int pos = 0;
                for(int k=0; k<prod1.size() && k<prod2.size(); k++)
                {
                    if(prod1[k] == prod2[k])
                    {
                        common.push_back(prod1[k]);
                        pos = k + 1;
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
                    vector <string> epsilon, s1, s2;
                    map<string, set< vector<string> > >::iterator it1;
                    it1 = modified.find(name);
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
            nt.productions = input.at(i).productions;
            nt.max_size = input.at(i).max_size;
            leftFactoring.push_back(nt);
        }
        else
        {
            set< vector<string> > m = it1->second;
            set< vector<string> >::iterator it2;
            nt.max_size = 0;
            for (it2 = m.begin(); it2 != m.end(); ++it2)
            {
                nt.productions.push_back(*it2);
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
                    nt.productions.push_back(input.at(i).productions.at(k));
                    if(input.at(i).productions.at(k).size() > nt.max_size)
                    {
                        nt.max_size = input.at(i).productions.at(k).size();
                    }
                }
            }
            leftFactoring.push_back(nt);
        }
    }
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
                new_NonTerminal.name = name+'\''+'\'';
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

void get_first(int i, int j)
{
    string nonTerminal = all_nonTerminals.at(i).name;
    string str = all_nonTerminals.at(i).productions.at(j).at(0);
    if (is_terminal(str))
    {
        first[nonTerminal].insert(str);
        pairs x = make_pair(str, all_nonTerminals.at(i).productions.at(j));
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
    set <pairs>::iterator it;
    set<pairs> s1 = firstForTable[str];
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

map<string, map<string, vector<string>>> build_table()
{

    map<string, map<string, vector<string>>> table;
    map<string, set<pairs> >::iterator TerminalIterator;
    set <pairs>::iterator FirstIterator;
    for(TerminalIterator = firstForTable.begin(); TerminalIterator != firstForTable.end(); ++TerminalIterator)
    {
        string terminal = TerminalIterator->first;
        set<pairs> firstSet = TerminalIterator->second;
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

void leftMostDerivation(map<string, map<string, vector<string> > > table)
{
    std::ofstream outfile;
    outfile.open("output.txt", std::ios_base::app); // append instead of overwrite
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
    leftMostDerivation(m);
    return 0;
}
