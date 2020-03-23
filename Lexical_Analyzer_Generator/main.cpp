#include <iostream>
#include <bits/stdc++.h>

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

    vector<transtion> get_tran()
    {
        return transitions;
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
    }

    int get_finalState()
    {
        return final_state;
    }
};

map<string, FA> regular_Definitions;
map<string, FA> regular_Expression;
map<string, FA> key_words;
map<char, FA> punctuations;

FA concatenation (FA a, FA b)
{
    FA result;
    result.set_vertices((a.get_no_vertices() + b.get_no_vertices()));
    result.set_startState(a.get_startState());
    result.set_finalState(a.get_no_vertices() + b.get_no_vertices()-1);
    for (int i = 0 ; i < a.get_tran().size(); i++)
    {
        struct transtion tran;
        tran = a.get_tran().at(i);
        result.set_transtions(tran.vertex_from,tran.vertex_to,tran.symbol);
    }
    result.set_transtions(a.get_finalState(),b.get_startState()+a.get_no_vertices(),'L');
    for (int i = 0 ; i < b.get_tran().size(); i++)
    {
        struct transtion tran;
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
        struct transtion tran;
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
        struct transtion tran;
        tran = a.get_tran().at(i);
        result.set_transtions(tran.vertex_from+1,tran.vertex_to+1,tran.symbol);
    }
    result.set_transtions(a.get_finalState()+1,result.get_finalState(),'L');
    result.set_transtions(result.get_finalState()-1,result.get_startState()+1,'L');
    return result;
}

FA Union (FA a, FA b)
{
    FA result;
    result.set_vertices(a.get_no_vertices()+b.get_no_vertices()+2);
    result.set_startState(a.get_startState());
    result.set_finalState(result.get_no_vertices()-1);
    result.set_transtions(result.get_startState(),a.get_startState()+1,'L');
    for(int i=0; i<a.get_tran().size(); i++)
    {
        struct transtion tran;
        tran = a.get_tran().at(i);
        result.set_transtions(tran.vertex_from+1,tran.vertex_to+1,tran.symbol);
    }
    result.set_transtions(a.get_finalState()+1,result.get_finalState(),'L');
    result.set_transtions(result.get_startState(),b.get_startState()+a.get_no_vertices()+1,'L');
    for(int i=0; i<b.get_tran().size(); i++)
    {
        struct transtion tran;
        tran = b.get_tran().at(i);
        result.set_transtions(tran.vertex_from+a.get_no_vertices()+1,tran.vertex_to+a.get_no_vertices()+1,tran.symbol);
    }
    result.set_transtions(b.get_finalState()+a.get_no_vertices()+1,result.get_finalState(),'L');
    return result;
}

void keyWords(string input)
{
    if(input[0]=='{'){
        input.erase(input.begin() +0);
    }
    if(input[input.length()-1]=='}'){
        input.erase(input.begin() +input.length()-1);
    }
    istringstream ss(input);
    do
    {
        string word;
        ss >> word;
        if(word == ""){
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
    if(input[0]=='['){
        input.erase(input.begin() +0);
    }
    if(input[input.length()-1]==']'){
        input.erase(input.begin() +input.length()-1);
    }
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
FA NFAtoDFA (FA a)
{
    FA DFA;
    vector<tranistion> transitions = a.get_tran();
    vector<int> finalStates = a.get_final_to_DFA();
    vector<char> symbols;
    DFA.set_startState(0);
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
            struct transitions t = transitions.at(i);
            if(t.vertex_from == j && t.symbol == 'L')
            {
                elem.eq.push_back(t.vertex_to);
            }
        }
        size_elem ++;
    }
    elements.push_back(elem);

    int in=0;
    int increase=0;
    while(in<elements.size())
    {
        struct DFAelement e = elements.at(in);

        if(!e.mark)
        {
            e.mark= true;
            e.index=in;

            for(int sym=0; sym<symbols.size(); sym++)
            {
                char currentSymbol = symbols.at(sym);
                struct DFAelement eNew;
                eNew.mark=false;
                for(int j=0; j<e.eq.size(); j++)
                {
                    int from = e.eq.at(j);
                    for(int k =0; k< transitions.size(); k++)
                    {
                        struct transition t = transitions.at(k);
                        if(t.vertex_from == from && t.symbol == currentSymbol){
                            eNew.eq.push_back(t.vertex_to);
                        }
                    }
                }
                bool inElements = false;
                int indexNew=0;
                for(int elem=0;elem<elements.size();elem++){
                    struct DFAelement d = elements.at(elem);
                    if(d.eq.size() == eNew.eq.size()){
                        int tempSize=0;
                        for(int temp1=0;temp1<d.eq.size();temp1++){
                            for(int temp2=0;temp2<eNew.eq.size();temp2++){
                                if(d.eq.at(temp1) == eNew.eq.at(temp2)){
                                    tempSize++;
                                }
                            }
                        }
                        if(tempSize == d.eq.size()){
                            inElements = true;
                            indexNew= d.index;
                            break;
                        }
                    }
                }
                if(!inElements){ //if it is a new state then push it to elements
                    increase++;
                    eNew.index=increase;
                    elements.push_back(eNew);
                    bool fin = false; //check if this state is a final state or not
                    for(int finalS=0;finalS<eNew.eq.size();finalS++){
                        for(int final2=0;final2<finalStates.size();final2++){
                            if(eNew.eq.at(finalS) == finalStates.at(final2)){
                                fin = true;
                                break;
                            }
                        }
                    }
                    if(fin){
                       DFA.set_final_to_DFA(eNew.index);
                    }
                }
                else{
                    eNew.index=indexNew;
                }
                DFA.set_transtions(in,eNew.index,currentSymbol);
            }
        }
        in++;
    }
    DFA.set_vertices(elements.size());
}
int main()
{
    FA result;
    result.set_finalState(0);
    result.set_finalState(3);
    string word = "{geeks for";
    word.erase(std::remove(word.begin(), word.end(), '{'), word.end());
    cout << result.get_startState() << result.get_finalState() << word << endl;
    cout << "Hello world!" << endl;
    return 0;
}
