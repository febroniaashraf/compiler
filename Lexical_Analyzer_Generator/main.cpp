#include <iostream>
#include <bits/stdc++.h>
#include<algorithm>
using namespace std;

struct transition
{
    int vertex_from;
    int vertex_to;
    char symbol;
};
struct DFAelement
{
    int index;
    vector<int> eq;
    bool mark;
};

int dummyState;
class FA
{
public:
    int no_vertices;
    int final_state;
    int start_state;
    vector<int> final_To_DFA;
    vector<transition> transitions;
    vector<int> vertices;

    FA ()
    {
        no_vertices = 2;
        final_state = 1;
        start_state = 0;
    }
    void set_vertices(int number)
    {
        no_vertices = number;
        for(int i=0; i<no_vertices; i++)
        {
            vertices.push_back(i);
        }
    }

    void set_final_to_DFA(int state)
    {
        final_To_DFA.push_back(state);
    }
    vector<int> get_final_to_DFA()
    {
        return final_To_DFA;
    }
    void set_transtions(int from, int to, char tran_symbol)
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
        final_To_DFA.push_back(state);
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

map<string, FA> regular_Definitions;
map<string, FA> regular_Expression;
map<string, FA> key_words;
map<char, FA> punctuations;
vector<pair<string,int>> output_file;

FA concatenation (FA a, FA b)
{
    FA result;
    result.set_vertices((a.get_no_vertices() + b.get_no_vertices()));
    result.set_startState(a.get_startState());
    result.set_finalState(a.get_no_vertices() + b.get_no_vertices()-1);
    for (int i = 0 ; i < a.get_tran().size(); i++)
    {
        struct transition tran;
        tran = a.get_tran().at(i);
        result.set_transtions(tran.vertex_from,tran.vertex_to,tran.symbol);
    }
    result.set_transtions(a.get_finalState(),b.get_startState()+a.get_no_vertices(),'L');
    for (int i = 0 ; i < b.get_tran().size(); i++)
    {
        struct transition tran;
        tran = b.get_tran().at(i);
        result.set_transtions(tran.vertex_from+a.get_no_vertices(),tran.vertex_to+a.get_no_vertices(),tran.symbol);
    }
    return result;

}

FA kleene(FA a)
{
    FA result;
    result.set_vertices(a.get_no_vertices() + 2);
    result.set_startState(a.get_startState());
    result.set_finalState(a.get_finalState() + 2);
    result.set_transtions(result.get_startState(), a.get_startState() + 1, 'L');
    result.set_transtions(result.get_startState(), result.get_finalState(), 'L');
    for (int i = 0 ; i < a.get_tran().size(); i++)
    {
        struct transition tran;
        tran = a.get_tran().at(i);
        result.set_transtions(tran.vertex_from+1,tran.vertex_to+1,tran.symbol);
    }
    result.set_transtions(a.get_finalState()+1,result.get_finalState(),'L');
    result.set_transtions(result.get_finalState()- 1,result.get_startState() + 1,'L');
    return result;
}
FA positive_closure(FA a)
{
    FA result;
    result.set_vertices(a.get_no_vertices()+2);
    result.set_startState(a.get_startState());
    result.set_finalState(a.get_finalState()+2);
    result.set_transtions(result.get_startState(),a.get_startState()+1,'L');
    for(int i=0; i<a.get_tran().size(); i++)
    {
        struct transition tran;
        tran = a.get_tran().at(i);
        result.set_transtions(tran.vertex_from+1,tran.vertex_to+1,tran.symbol);
    }
    result.set_transtions(a.get_finalState()+1,result.get_finalState(),'L');
    result.set_transtions(result.get_finalState()-1,result.get_startState()+1,'L');
    return result;
}

FA Union (vector<FA> all)
{
    FA result;
    int sum = 0;
    for(int i=0; i<all.size(); i++)
    {
        sum = sum+ all.at(i).get_no_vertices();
    }
    result.set_vertices(sum+2);
    result.set_startState(0);
    result.set_finalState(sum+1);
    for(int i=0; i<all.size(); i++)
    {
        int start = 0;
        for(int j=0; j<i; j++)
        {
            start = start+all.at(j).get_no_vertices();
        }
        result.set_transtions(result.get_startState(), all.at(i).get_startState()+start+1, 'L');
        for(int k = 0; k<all.at(i).get_tran().size(); k++)
        {
            struct transition tran;
            tran = all.at(i).get_tran().at(k);
            result.set_transtions(tran.vertex_from+start+1,tran.vertex_to+start+1,tran.symbol);
        }
        result.set_transtions(all.at(i).get_finalState()+start+1,result.get_finalState(),'L');
    }
    return result;
}

void keyWords(string input)
{
    istringstream ss(input);
    do
    {
        string word;
        ss >> word;
        if(word == "")
        {
            continue;
        }
        FA key;
        key.set_vertices(word.length()+1);
        key.set_startState(0);
        key.set_finalState(word.length());
        for(int i=0; i<word.length(); i++)
        {
            key.set_transtions(i,i+1,word[i]);
        }
        key_words.insert(pair<string, FA>(word, key));
    }
    while (ss);
}

void punc(string input)
{
    for(int i=0; i<input.length(); i++)
    {
        FA key;
        if(input[i]==' ' || input[i]=='\\')
        {
            continue;
        }
        else
        {
            key.set_vertices(2);
            key.set_startState(0);
            key.set_finalState(1);
            key.set_transtions(key.get_startState(),key.get_finalState(),input[i]);
        }
        punctuations.insert(pair<char, FA>(input[i], key));
    }
}

FA range (FA a, FA b)
{
    char first = a.transitions[0].symbol;
    char last = b.transitions[0].symbol;
    vector<char> v_char;
    vector<FA> v_fa;
    for (char i = first ; i <= last ; i++)
    {
        v_char.push_back(i);
    }

    for(int i=0; i< v_char.size(); i++)
    {
        FA sym;
        sym.set_vertices(2);
        sym.set_startState(0);
        sym.set_finalState(1);
        sym.set_transtions(sym.get_startState(),sym.get_finalState(),v_char[i]);
        v_fa.push_back(sym);
    }
    FA result;
    vector<FA> all;
    for (int i = 0 ; i < v_fa.size() ; i++)
    {
        all.push_back(v_fa[i]);
    }
    result = Union (all);
    return result;
}
FA return_FA(string symbols)
{
    //cout << symbols << endl;
    FA re;
    map<string,FA>::iterator it;
    it = regular_Definitions.find(symbols); // check if symbol exists in Re DE map
    if(it != regular_Definitions.end())
    {
        re = it->second;
        return re;
    }
    else // if not exist so we need to calculate NFA for it
    {
        FA key;
        key.set_vertices(symbols.length()+1);
        key.set_startState(0);
        key.set_finalState(symbols.length());
        for(int i=0; i<symbols.length(); i++)
        {
            key.set_transtions(i,i+1,symbols[i]);
        }
        return key;
    }
}
void expression_to_NFA(string name, string expression, string type)
{
    stack<char> operators;
    stack<FA> finite_automata;
    bool enter = false, is_def = false;
    string symbols, check1 = "+*-|\)", check2="-|\)\(", check3="+*-|\)\(", check4="+*|\)\( ";
    if(type == "def")
    {
        is_def = true;
    }
    for(int i=0; i<expression.length(); i++)
    {
        char current = expression[i], next = expression[i+1], before = expression[i-1], before2 = expression[i-2];

        if((i==0 && current==' ')||(i== expression.length()-1 && current==' '))
        {
            continue;
        }

        if(current == '\\')
        {
            symbols+= next;
            i++;
            if(symbols!="")
            {
                if(i==expression.length()-1)
                {

                    finite_automata.push(return_FA(symbols));
                    symbols="";
                }
            }
            continue;
        }
        else if(current==' ' || current==')')
        {
            if(current==' ')
            {
                if((check1.find(next) == std::string::npos) && before2!='\\') // not + ,* ,- ,| ,)
                {
                    if(check2.find(before) == std::string::npos) // not - ,| ,) ,(
                    {
                        operators.push('#');
                    }
                }
                else if((check3.find(next) == std::string::npos) && before2=='\\') // not +, *, -, |, ), (
                {
                    operators.push('#');
                }
            }
            if(symbols!="")
            {
                finite_automata.push(return_FA(symbols));
            }
            symbols = "";
        }
        else if(current!=')' && (next=='+' || next=='*' || next=='-' || next=='|'))// ex: digit+ or digit*
        {
            symbols+= current;
            enter = true;
            if(symbols!="")
            {
                finite_automata.push(return_FA(symbols));
            }
            symbols = "";
        }
        if(current != ' ' && check4.find(current) == std::string::npos) // not +, *, |, ), (, ' '
        {
            if (current =='-' && is_def) // ex: expression = A - Z , current = -
            {
                operators.push(current);
            }
            else
            {
                if(!enter) // ex: expression = digit , current = i
                {
                    symbols+= current;
                    if(i==expression.length()-1) // ex: expression = digit , current = t
                    {
                        if(symbols!="")
                        {
                            finite_automata.push(return_FA(symbols));
                        }
                    }
                }
                else
                {
                    enter = false;
                }
            }
        }
        else
        {
            if(current=='*')
            {
                FA a = finite_automata.top();
                finite_automata.pop();
                finite_automata.push(kleene(a));
            }
            else if(current=='+')
            {
                FA a = finite_automata.top();
                finite_automata.pop();
                finite_automata.push(positive_closure(a));
            }
            else if(current=='|' || current=='(')
            {
                if (operators.size()!=0 && operators.top() == '-')
                {
                    FA one = finite_automata.top();
                    finite_automata.pop();
                    FA two = finite_automata.top();
                    finite_automata.pop();
                    finite_automata.push(range(two, one));
                    operators.pop();
                }
                operators.push(current);
            }
            else if(current==')')
            {
                while(operators.top()!='(')
                {
                    if(operators.top()=='|')
                    {
                        vector<FA> all;
                        while(operators.size()!=0&&operators.top()=='|')
                        {
                            all.insert(all.begin(),finite_automata.top());
                            finite_automata.pop();
                            operators.pop();
                        }

                        all.insert(all.begin(),finite_automata.top());
                        finite_automata.pop();
                        finite_automata.push(Union(all));
                    }
                    else if(operators.top()=='#')
                    {
                        FA one = finite_automata.top();
                        finite_automata.pop();
                        FA two = finite_automata.top();
                        finite_automata.pop();
                        finite_automata.push(concatenation(two,one));
                        operators.pop();
                    }
                    else if(operators.top()=='-')
                    {
                        FA one = finite_automata.top();
                        finite_automata.pop();
                        FA two = finite_automata.top();
                        finite_automata.pop();
                        finite_automata.push(range(two, one));
                        operators.pop();
                    }
                }
                operators.pop();
            }
        }
    }
    if(finite_automata.size()>1&& operators.size()>0)
    {
        while(operators.size()!=0)
        {
            if(operators.top()=='|')
            {
                vector<FA> all;
                while(operators.size()!=0&& operators.top()=='|')
                {
                    all.insert(all.begin(),finite_automata.top());
                    finite_automata.pop();
                    operators.pop();
                }
                all.insert(all.begin(),finite_automata.top());
                finite_automata.pop();
                finite_automata.push(Union(all));
            }
            else if(operators.top()=='#')
            {
                FA one = finite_automata.top();
                finite_automata.pop();
                FA two = finite_automata.top();
                finite_automata.pop();
                finite_automata.push(concatenation(two,one));
                operators.pop();
            }
            else if(operators.top()=='-')
            {
                FA one = finite_automata.top();
                finite_automata.pop();
                FA two = finite_automata.top();
                finite_automata.pop();
                finite_automata.push(range(two, one));
                operators.pop();
            }
        }
    }
    if(type == "re")
    {
        regular_Expression.insert(pair<string, FA>(name,finite_automata.top()));
        finite_automata.pop();
    }
    else if(type == "def")
    {
        regular_Definitions.insert(pair<string, FA>(name,finite_automata.top()));
        finite_automata.pop();
    }
}

void read_file (const char* input_file)
{
    fstream newfile;
    newfile.open(input_file,ios::in); //open a file to perform read operation using file object
    if (newfile.is_open())    //checking whether the file is open
    {
        string line;
        while(getline(newfile, line))  //read data from file object and put it into string.
        {
            string word = "";
            for (std::string::size_type i = 0; i < line.size(); i++)
            {
                if (line[i] == '{')  // keywords
                {
                    line.erase(line.begin() + 0);
                    line.erase(line.end() - 1);
                    keyWords(line);
                    break;
                }
                else if (line[i]  == '[')   // punctuation
                {
                    line.erase(line.begin() + 0);
                    line.erase(line.end() - 1);
                    punc(line);
                    break;
                }
                else if (line[i] == ':')   // regular expression
                {
                    line.erase(line.begin() + 0, line.begin() + i + 1);
                    word.erase (std::remove (word.begin(), word.end(), ' '), word.end());
                    expression_to_NFA(word, line, "re");
                    break;
                }
                else if (line[i] == '=')   // regular definition
                {
                    line.erase(line.begin() + 0, line.begin() + i + 1);
                    // line.erase (std::remove (line.begin(), line.end(), ' '), line.end());
                    word.erase (std::remove (word.begin(), word.end(), ' '), word.end());
                    expression_to_NFA(word, line, "def");
                    break;
                }
                else   //read the label of a regular expression or a regular definition
                {
                    word = word + line[i];
                }
            }
        }
        newfile.close(); //close the file object.
    }
}

FA Union_NFA(vector<FA> all)
{
    FA result;
    int sum = 0;
    for(int i=0; i<all.size(); i++)
    {
        sum = sum+ all.at(i).get_no_vertices();
    }
    result.set_vertices(sum+1);
    result.set_startState(0);
    for(int i=0; i<all.size(); i++)
    {
        int start = 0;
        for(int j=0; j<i; j++)
        {
            start = start+all.at(j).get_no_vertices();
        }
        result.set_transtions(result.get_startState(), all.at(i).get_startState()+start+1, 'L');
        for(int k = 0; k<all.at(i).get_tran().size(); k++)
        {
            struct transition tran;
            tran = all.at(i).get_tran().at(k);
            result.set_transtions(tran.vertex_from+start+1,tran.vertex_to+start+1,tran.symbol);
        }
        output_file.at(i).second = all.at(i).get_finalState()+start+1;
        result.set_final_to_DFA(all.at(i).get_finalState()+start+1);
    }
    return result;
}

FA language()
{
    FA result;
    vector<FA> all;
    map<string, FA> grammer;
    map<string, FA>::iterator itr;
    for (itr = regular_Expression.begin(); itr != regular_Expression.end(); ++itr)
    {
        grammer.insert(pair<string,FA>(itr->first,itr->second));
    }
    for (itr = key_words.begin(); itr != key_words.end(); ++itr)
    {
        grammer.insert(pair<string,FA>(itr->first,itr->second));
    }
    map<char, FA>::iterator itr1;
    for (itr1 = punctuations.begin(); itr1 != punctuations.end(); ++itr1)
    {
        string str = "";
        str += itr1->first;
        grammer.insert(pair<string,FA>(str,itr1->second));
    }
    for (itr = grammer.begin(); itr != grammer.end(); ++itr)
    {
        pair<string, int> o;
        o.first = itr->first;
        o.second = itr->second.get_finalState();
        output_file.push_back(o);
        all.push_back(itr->second);
    }
    result = Union_NFA(all);
    return result;
}

FA NFAtoDFA (FA a)
{
    FA DFA;

    vector<transition> transitions = a.get_tran();
    vector<int> finalStates = a.get_final_to_DFA();
    vector<char> symbols;
    DFA.set_startState(0);
    for(int i=0; i<transitions.size(); i++) //GET ALL SYMBOLS IN NFA
    {
        char s = transitions.at(i).symbol;
        bool contain= false;
        for(int j=0; j<symbols.size(); j++)
        {
            if(symbols.at(j) == s)
            {
                contain=true;
                break;
            }
        }
        if(!contain)
        {
            symbols.push_back(s);
        }
    }

    vector<DFAelement> elements;
    struct DFAelement elem;
    elem.index =0;
    elem.mark= false;
    elem.eq.push_back(a.get_startState());
    int size_elem =0;
    while(size_elem < elem.eq.size())  //get S0 closure
    {
        int j = elem.eq.at(size_elem);
        for(int i =0; i< transitions.size(); i++)
        {
            struct transition t = transitions.at(i);
            if(t.vertex_from == j && t.symbol == 'L')
            {
                bool inS0 = false;
                for(int inS=0; inS<elem.eq.size(); inS++)
                {
                    if(elem.eq.at(inS) ==t.vertex_to )
                    {
                        inS0 = true;
                        break;
                    }
                }
                if(!inS0)
                {
                    elem.eq.push_back(t.vertex_to);

                }
            }
        }
        size_elem ++;
    }
    elements.push_back(elem);
    int in=0;
    int increase=0;
    while(in<elements.size()) //CALCULATE THE NEW STATES
    {
        struct DFAelement e = elements.at(in);

        if(!e.mark) //CHECK IF WE TAKE IT BEFORE OR NOT
        {
            e.mark= true;
            e.index=in;

            for(int sym=0; sym<symbols.size(); sym++) //GET THE NEXT STATE FOR EVERY SYMBOL
            {
                char currentSymbol = symbols.at(sym);
                if(currentSymbol != 'L')
                {
                    struct DFAelement eNew;
                    eNew.mark=false;
                    for(int j=0; j<e.eq.size(); j++)
                    {
                        int from = e.eq.at(j);
                        bool inE = false;
                        for(int k =0; k< transitions.size(); k++)
                        {
                            struct transition t = transitions.at(k);
                            if(t.vertex_from == from && t.symbol == currentSymbol)
                            {
                                for(int s=0; s<eNew.eq.size(); s++)
                                {
                                    if(eNew.eq.at(s) == t.vertex_to)
                                    {
                                        inE=true;
                                        break;
                                    }
                                }
                                if(!inE)
                                {
                                    eNew.eq.push_back(t.vertex_to);
                                }
                            }
                        }
                    }

                    int j =0;
                    while(j < eNew.eq.size())  //get S closure
                    {
                        int from = eNew.eq.at(j);
                        for(int k =0; k< transitions.size(); k++)
                        {
                            struct transition t = transitions.at(k);
                            if(t.vertex_from == from && t.symbol == 'L')
                            {
                                bool inE = false;
                                for(int s=0; s<eNew.eq.size(); s++)
                                {
                                    if(eNew.eq.at(s) == t.vertex_to)
                                    {
                                        inE=true;
                                        break;
                                    }
                                }
                                if(!inE)
                                {
                                    eNew.eq.push_back(t.vertex_to);
                                }
                            }
                        }
                        j ++;
                    }

                    bool inElements = false;
                    int indexNew=0;
                    for(int elem=0; elem<elements.size(); elem++) //CHECK IF THIS STATE ADDED BEFORE TO ELEMENTS OR NOT
                    {
                        struct DFAelement d = elements.at(elem);
                        if(d.eq.size() == eNew.eq.size())
                        {
                            int tempSize=0;
                            for(int temp1=0; temp1<d.eq.size(); temp1++)
                            {
                                for(int temp2=0; temp2<eNew.eq.size(); temp2++)
                                {
                                    if(d.eq.at(temp1) == eNew.eq.at(temp2))
                                    {
                                        tempSize++;
                                    }
                                }
                            }
                            if(tempSize == d.eq.size())
                            {
                                inElements = true;
                                indexNew= d.index;
                                break;
                            }
                        }
                    }
                    if(!inElements)  //if it is a new state then push it to elements
                    {
                        increase++;
                        eNew.index=increase;
                        elements.push_back(eNew);
                        if(eNew.eq.size()==0)
                        {
                            dummyState = increase;
                        }
                        bool fin = false; //check if this state is a final state or not
                        for(int finalS=0; finalS<eNew.eq.size(); finalS++)
                        {
                            for(int final2=0; final2<finalStates.size(); final2++)
                            {
                                if(eNew.eq.at(finalS) == finalStates.at(final2))
                                {
                                    fin = true;
                                    break;
                                }
                            }
                        }
                        if(fin)
                        {
                            DFA.set_final_to_DFA(eNew.index);
                        }
                    }
                    else
                    {
                        eNew.index=indexNew;
                    }
                    DFA.set_transtions(in,eNew.index,currentSymbol);
                }
            }
        }
        in++;
    }
     cout <<"done1\n";
    DFA.set_vertices(elements.size());
    return DFA;
}

void output()
{


    FA result;
    FA result2;
    map<string, FA>::iterator itr;
    map<string, FA>::iterator itr2;
    FA re = language();
    re.display();

}

vector<int> getNonFinalStates(vector<int> first, vector<int> second)
{
    std::vector<int> v(first.size()+second.size());
    std::vector<int>::iterator it;

    std::sort (first.begin(),first.end());
    std::sort (second.begin(),second.end());

    it=std::set_difference (first.begin(), first.end(), second.begin(), second.end(), v.begin());
    v.resize(it-v.begin());
    return v;

}
vector<char> getInputs(vector<transition> transitions)
{
    vector<char> symbols;
    for(int i=0; i<transitions.size(); i++)
    {
        char s = transitions.at(i).symbol;
        bool contain= false;
        for(int j=0; j<symbols.size(); j++)
        {
            if(symbols.at(j) == s)
            {
                contain=true;
                break;
            }
        }
        if(!contain)
        {
            symbols.push_back(s);
        }
    }
    return symbols;
}

string findNextStats (int state, vector<transition> transitions, vector<char> inputs,map<int, int> keyOfStates)
{
    std::stringstream mySS;
    for(int j=0; j<inputs.size(); j++)
    {
        for(int i=0; i<transitions.size(); i++)
        {
            if(transitions[i].vertex_from == state && transitions[i].symbol == inputs.at(j))
            {
                mySS << keyOfStates[transitions[i].vertex_to];
            }
        }
    }
    return mySS.str();
}

void updateMapValues(int value,vector<int> v, std::map<int, int> &keyOfStates)
{
    for(int i=0; i<v.size(); i++)
    {
        keyOfStates[v.at(i)] = value;
    }
}

map<int, vector<int> > minimizaion (FA DFA)
{
    int numberOfSets = 2;
    int counter = 0;
    int whichPartition = 1;

    map<string,vector<int> > pre_result;
    std::map<int, int> keyOfStates;
    map<int,vector<int> > partitions;
    vector<transition> transitions = DFA.get_tran();
    vector<int> finalStates = DFA.get_final_to_DFA();
    vector<int> vertices = DFA.get_vertices();
    vector<int> NonFinalStates = getNonFinalStates(vertices,finalStates);
    vector<char> inputs = getInputs(transitions);
    vector<int> currntVector = NonFinalStates;
    vector<int> mySS;

    updateMapValues(1,NonFinalStates,keyOfStates);
    updateMapValues(2,finalStates,keyOfStates);
    partitions.insert(pair<int, vector<int> >(1,NonFinalStates));
    partitions.insert(pair<int, vector<int> >(2,finalStates));
    mySS.push_back(1);
    mySS.push_back(2);

    while(currntVector.size() > counter)
    {
        string nextStates = findNextStats (currntVector[counter], transitions, inputs, keyOfStates);
        pre_result[nextStates].push_back(currntVector.at(counter));
        counter++;
        if(counter >= currntVector.size())
        {
            if (pre_result.size() > 1)
            {
                for (std::map<string,vector<int> >::iterator it = pre_result.begin(); it!=pre_result.end(); ++it)
                {
                    numberOfSets++;
                    partitions.insert(pair<int,vector<int> >(numberOfSets, it->second));
                }
                partitions[0].push_back(whichPartition);
            }
            mySS.erase(mySS.begin());
            if(!mySS.empty())
            {
                whichPartition= mySS.at(0);
                currntVector = partitions[whichPartition];
            }
            else if(!partitions[0].empty())
            {
                for(int i = 0; i< partitions[0].size(); i++)
                    partitions.erase(partitions[0].at(i));

                partitions.erase(0);
                for (std::map<int,vector<int> >::iterator it = partitions.begin(); it!=partitions.end(); ++it)
                {
                    updateMapValues(it->first,it->second,keyOfStates);
                    mySS.push_back(it->first);
                }
                whichPartition = partitions.begin()->first;
                currntVector = partitions[whichPartition];
            }
            else
            {
                partitions.erase(0);
                break;
            }
            pre_result.clear();
            counter = 0;
        }
    }
    return partitions;
}

FA  minimizedTable (map<int, vector<int> > partitions,FA DFA)
{
    FA result;
    std::map<int, int> keyOfStates;
    vector<transition> transitions = DFA.get_tran();
    vector<int> finalStates= DFA.get_final_to_DFA();
    for (std::map<int,vector<int> >::iterator it = partitions.begin(); it!=partitions.end(); ++it)
    {
        if(it->second.size() > 1)
        {
            vector<int>  v ;
            v.insert(v.end(),it->second.begin()+1,it->second.end());
            updateMapValues(it->second[0],v,keyOfStates);
        }
    }
    for(int i=0; i<transitions.size(); i++)
    {
        if(keyOfStates.count(transitions[i].vertex_from))
        {
            transitions.erase(transitions.begin()+ i);
            i--;
        }
        else if(keyOfStates.count(transitions[i].vertex_to))
        {
            transitions[i].vertex_to = keyOfStates[transitions[i].vertex_to];
        }
    }
    for(int i=0; i<finalStates.size(); i++)
    {
        if(!keyOfStates.count(finalStates[i]))
        {
            result.set_final_to_DFA(finalStates[i]);
        }
    }

    result.set_vertices(keyOfStates.size());
    // result.set_tran(transitions);
    return result;
}
int main()
{
    read_file("my.txt");
    FA result = language();
    FA dfa = NFAtoDFA(result);
    //map<int, vector<int>> mini = minimizaion(dfa);
    //FA miniTable = minimizedTable(mini, dfa);
    dfa.display();
    return 0;
}
