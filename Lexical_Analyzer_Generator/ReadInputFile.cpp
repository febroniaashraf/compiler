#include "ReadInputFile.h"
#include "recource.h"
#include "ClassFA.h"
#include "Functions.h"

map<string, FA> regular_Definitions;
map<string, FA> regular_Expression;
map<string, FA> key_words;
map<char, FA> punctuations;

/**-----------------------------------------------------------------------------------------
 * USEAGE : build FA to input keywords and adding them to map keywords
 * Take   : string that includes keywords
 *-----------------------------------------------------------------------------------------*/

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

/**-----------------------------------------------------------------------------------------
 * USEAGE : build FA to input punctuations and adding them to map punctuations
 * Take   : string that includes punctuations
 *-----------------------------------------------------------------------------------------*/

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

/**-----------------------------------------------------------------------------------------
 * USEAGE : checks if input string is in map that includes regular definitions, if not included in map then build the FA of input string
 * Take   : string name
 * RETURN : FA of input string
 *-----------------------------------------------------------------------------------------*/

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

/**-----------------------------------------------------------------------------------------
 * USEAGE : build NFA to input string name if it is regular expression or definition depending on input type
 * Take   : name of regular expression or definition, expression of input name, type if it is expression or definition
 *-----------------------------------------------------------------------------------------*/

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
            if(next == 'L')
            {
                symbols+= '^';
            }
            else
            {
                symbols+= next;
            }
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

/**-----------------------------------------------------------------------------------------
 * USEAGE : build final NFA by applying union operation to keywords, punctuations and regular expression
 * RETURN : final NFA of input language
 *-----------------------------------------------------------------------------------------*/

FA language()
{
    FA result;
    vector<FA> all;
    vector<string> names;
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
        all.push_back(itr->second);
        names.push_back(itr->first);
    }
    result = Union_NFA(all, names);
    return result;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : read input file that includes language and fill maps
 * Take   : name of input file
 *-----------------------------------------------------------------------------------------*/

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
