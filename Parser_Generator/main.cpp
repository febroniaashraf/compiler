#include <iostream>
#include <bits/stdc++.h>
#include <set>
#include <regex>
#include <iterator>
#include<algorithm>

using namespace std;
typedef pair<string,vector<string>> pairs;
struct Non_terminal
{
    string name;
    vector<vector<string> > productions;
};

vector<Non_terminal> all_nonTerminals;
vector<string> terminals;
map<string, set<string> > first;
map<string, set<string> > follow;
map<string, set<pairs> > firstForTable;
map<string,vector<string> > terminalsColumn;
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
void get_follow(string name)
{
    for(int i=0;i<all_nonTerminals.size();i++)
    {
        for(int j=0;j<all_nonTerminals.at(i).productions.size();j++)
        {
            vector<string> prod = all_nonTerminals.at(i).productions.at(j);
            bool flag = false;
            for(int k = 0;k<prod.size();k++)
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
    for(int i=0;i<all_nonTerminals.size();i++)
    {
        string name = all_nonTerminals.at(i).name;
        if(i==0)
        {
            follow[name].insert("$");
        }
        get_follow(name);
    }
}

void left_recurion(vector<Non_terminal> leftFactoring)
{
    vector<Non_terminal> final_nonTerminals;
    for(int i=0; i<leftFactoring.size();i++)
    {
        string name = leftFactoring.at(i).name;
        bool found = false;
        for(int j=0;j<leftFactoring.at(i).productions.size();j++)
        {
            string prod = leftFactoring.at(i).productions.at(j).at(0);
            if(prod == name)
            {
               found = true;
               vector<string> remove_production = leftFactoring.at(i).productions.at(j);
               struct Non_terminal new_NonTerminal, current_NonTerminal;
               current_NonTerminal.name = name;
               new_NonTerminal.name = name+'\'';
               vector<string> current_production;
               if(leftFactoring.at(i).productions.size()==1)
               {
                   current_production.push_back(new_NonTerminal.name);
                   current_NonTerminal.productions.push_back(current_production);
               }
               for(int k=0;k<leftFactoring.at(i).productions.size();k++)
               {
                    if(j!=k)
                    {
                        current_production = leftFactoring.at(i).productions.at(k);
                        current_production.push_back(new_NonTerminal.name);
                        current_NonTerminal.productions.push_back(current_production);
                    }

               }
               final_nonTerminals.push_back(current_NonTerminal);
               remove_production.erase(remove_production.begin());
               remove_production.push_back(new_NonTerminal.name);
               vector<string> epsilon;
               epsilon.push_back("^");
               new_NonTerminal.productions.push_back(remove_production);
               new_NonTerminal.productions.push_back(epsilon);
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
map<string, map<string, vector<string>>> build_table(){

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
            table[terminal][p.first] = p.second;
        }
    }

    map<string, set<string> >::iterator TerminalIterator2;
    set <string>::iterator followIterator;
    for(TerminalIterator2 = follow.begin(); TerminalIterator2 != follow.end(); ++TerminalIterator2)
    {
        string terminal = TerminalIterator2->first;
        set<string> followSet = TerminalIterator2->second;
        for (followIterator = followSet.begin(); followIterator != followSet.end(); ++followIterator)
        {
            string str = *followIterator;
            if(str != "^"){
                    bool epsilon= false;
                int n = get_nonTreminal_byName(str);
                struct Non_terminal non = all_nonTerminals.at(n);
                for(int i=0;i< non.productions.size();i++){
                    if(non.productions.at(i).at(0) == "^"){
                        epsilon = true;
                        break;
                    }
                }
                vector<string> v ;
               if(epsilon){
                    v.push_back("^");
                }
               else{
                    v.push_back("synch");
               }
               table[terminal][str] = v;
            }
        }

    }
    return table;
}
void print_table(map<string, map<string, vector<string>>> table){
    map<string, map<string, vector<string>>>::iterator it1;
    map<string, vector<string>>::iterator it2;
    for(it1 = table.begin(); it1 != table.end(); ++it1)
    {
        std::cout << "terminal : "<< it1->first << endl;
         map<string, vector<string>> s = it1->second;
        for (it2 = s.begin(); it2 != s.end(); ++it2)
        {
            string str = it2->first;
            vector<string> str2 = it2->second;
            cout << str << " :p: ";
            for(int i=0;i<str2.size();i++){
             cout<< str2.at(i)<< " ";
            }
             cout << ", ";
        }
        cout << endl;
        cout << "----------------------------" << endl;
    }
}

 queue<string> readOutAsIn(){
 string line;
  queue<string> queueInputs;
  ifstream myfile ("C:\\Users\\Osman\\Documents\\GitHub\\compiler\\Lexical_Analyzer_Generator\\output.txt");
  regex re("(.*)( --> )(.*)");
  smatch match;
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
    if (regex_search(line, match, re) == true) {
      if(match.str(3) == "assign"){
        queueInputs.push("=");
      }else{
      queueInputs.push(match.str(3));
      }
    cout<< queueInputs.back()<< " ";

    }
    }
    myfile.close();
  }
  queueInputs.push("$");
  return queueInputs;
}

map<string,string> leftMostDerivation(map<string, map<string, vector<string> > > table){
   map<string, string> result;
   std::queue<std::string> queueInputs = readOutAsIn();
   stack<string> stackProc;
   int counter = 20;
   stackProc.push("$");
   stackProc.push(all_nonTerminals.at(0).name);
   while(/*queueInputs.front()*/counter != 0){
    if(is_terminal(stackProc.top())){
        if(stackProc.top() == queueInputs.front()){
            cout<< stackProc.top() << " : "<< queueInputs.front()<< endl;
            stackProc.pop();
            queueInputs.pop();
            if(stackProc.top() == "$"){
                cout<< "error2"<< endl;
                break;
            }
        }
    }else {
        if(stackProc.top() == "^"){
            stackProc.pop();
        }else if(stackProc.top() == "synch"){
            cout<< "error3"<< endl;
            stackProc.pop();
        }else{
        vector<string> ss = table[stackProc.top()][queueInputs.front()];
         cout<<"top before pop  "<<ss.size()<<" "<<stackProc.top() <<" "<< queueInputs.front() << endl;
         if(ss.size() != 0){
        stackProc.pop();
        for(int i = ss.size() - 1;i >= 0; i--){
            cout<<ss.at(i)<< endl;
        stackProc.push(ss.at(i));
        }
        } else {
         cout<<queueInputs.front()<< "error excess"  <<endl;
         queueInputs.pop();
        }
        }
    }
    counter--;
   }
   while(stackProc.top() != "$"){
        if(is_terminal(stackProc.top())){
        cout<<stackProc.top()<< " missing "  <<endl;
        }
        stackProc.pop();
   }

   return result;
}

    int main()
{
    read_inputFile("input.txt");
    set_first();
    set_follow();
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
    cout << "----------------------------" << endl;
    map<string, set<string> >::iterator it3;
    set <string>::iterator it4;
    for(it3 = follow.begin(); it3 != follow.end(); ++it3)
    {
        std::cout << it3->first << endl;
        set<string> s = it3->second;
        for (it4 = s.begin(); it4 != s.end(); ++it4)
        {
            string str = *it4;
            cout << str << ", ";
        }
        cout << endl;
        cout << "----------------------------" << endl;
    }
        cout << "----------------------------" << endl;

    map<string, map<string, vector<string>>> m = build_table();
    print_table(m);

leftMostDerivation(m);

    return 0;
}


