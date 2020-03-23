#include <iostream>
#include <bits/stdc++.h>

using namespace std;

struct transtion
{
    int vertex_from;
    int vertex_to;
    char symbol;
};

class FA
{
public:
    int no_vertices;
    int final_state;
    int start_state;
    vector<int> final_To_DFA;
    vector<transtion> transitions;
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
        struct transtion tran;
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
    result.set_vertices((a.get_no_vertices() + b.get_no_vertices()) - 1);
    result.set_startState(a.get_startState());
    result.set_finalState(b.get_finalState());
    for (int i = 0 ; i < a.get_tran().size(); i++)
    {
        struct transtion tran;
        tran = a.get_tran().at(i);
        result.set_transtions(tran.vertex_from,tran.vertex_to,tran.symbol);
    }
    result.set_transtions(a.get_finalState(),b.get_startState(),'L');
    for (int i = 0 ; i < b.get_tran().size(); i++)
    {
        struct transtion tran;
        tran = a.get_tran().at(i);
        result.set_transtions(tran.vertex_from,tran.vertex_to,tran.symbol);
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
}

void keyWords(string input)
{
    input.erase(std::remove(input.begin(), input.end(), '{'), input.end());
    input.erase(std::remove(input.begin(), input.end(), '}'), input.end());
    istringstream ss(input);
    do
    {
        string word;
        ss >> word;
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
    input.erase(std::remove(input.begin(), input.end(), '['), input.end());
    input.erase(std::remove(input.begin(), input.end(), ']'), input.end());
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
