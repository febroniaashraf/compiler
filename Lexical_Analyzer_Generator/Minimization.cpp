#include "Minimization.h"
#include "recource.h"
#include "ClassFA.h"

map<int,vector<string> > Mini;

/**-----------------------------------------------------------------------------------------
 * USEAGE : Build the follow set of each nonterminal
 * Take   : production to each nonterminal
 * RETURN :
 *-----------------------------------------------------------------------------------------*/

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

/**-----------------------------------------------------------------------------------------
 * USEAGE : Build the follow set of each nonterminal
 * Take   : production to each nonterminal
 * RETURN :
 *-----------------------------------------------------------------------------------------*/

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

/**-----------------------------------------------------------------------------------------
 * USEAGE : Build the follow set of each nonterminal
 * Take   : production to each nonterminal
 * RETURN :
 *-----------------------------------------------------------------------------------------*/

string findNextStats (int state, vector<transition> transitions, vector<char> inputs,map<int, int> keyOfStates)
{
    std::stringstream mySS;
    for(int j=0; j<inputs.size(); j++)
    {
        for(int i=0; i<transitions.size(); i++)
        {
            if(transitions[i].vertex_from == state && transitions[i].symbol == inputs.at(j))
            {
                mySS << keyOfStates[transitions[i].vertex_to] << "^";
            }
        }
    }
    return mySS.str();
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : Build the follow set of each nonterminal
 * Take   : production to each nonterminal
 *-----------------------------------------------------------------------------------------*/

void updateMapValues(int value,vector<int> v, std::map<int, int> &keyOfStates)
{
    for(int i=0; i<v.size(); i++)
    {
        keyOfStates[v.at(i)] = value;
    }
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : Build the follow set of each nonterminal
 * Take   : production to each nonterminal
 * RETURN :
 *-----------------------------------------------------------------------------------------*/

map<int, vector<int> > minimizaion (FA DFA)
{
    int numberOfSets = 1;
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
    map<int ,int > mmap;
    updateMapValues(1,NonFinalStates,keyOfStates);
    partitions.insert(pair<int, vector<int> >(1,NonFinalStates));
    mySS.push_back(1);

    vector<vector<int> > finalStatesRelated;
     for(int i = 0; i < finalStates.size(); i++){
      if(!mmap.count(i)){
        vector<int> v;
        v.push_back(finalStates[i]);
        mmap[i] = 1;
        for(int j = 0; j < finalStates.size(); j++){
           if(!mmap.count(j) && finalStatesMap[finalStates[i]] ==  finalStatesMap[finalStates[j]]) {
                v.push_back(finalStates[j]);
                mmap[j] = 1;
        }
        }
        finalStatesRelated.push_back(v);
        }
    }
         for(int i = 0; i < finalStatesRelated.size(); i++){
                numberOfSets++;
                updateMapValues(numberOfSets,finalStatesRelated[i],keyOfStates);
                partitions.insert(pair<int, vector<int> >(numberOfSets,finalStatesRelated[i]));
              mySS.push_back(numberOfSets);
        }

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

/**-----------------------------------------------------------------------------------------
 * USEAGE : Build the follow set of each nonterminal
 * Take   : production to each nonterminal
 * RETURN :
 *-----------------------------------------------------------------------------------------*/

FA  minimizedTable (map<int, vector<int> > partitions,FA DFA)
{
    FA result;
    std::map<int, int> keyOfStates;
    vector<transition> transitions = DFA.get_tran();
    vector<int> finalStates= DFA.get_final_to_DFA();

    vector<int> vertices= DFA.get_vertices();
    result.set_startState(DFA.get_startState());
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
            vector<string>  v ;
            v.push_back(finalStatesMap[finalStates[i]]);
            Mini.insert(pair<int,vector<string> >(finalStates[i],v));
            result.set_final_to_DFA(finalStates[i]);
        } else {
            Mini[keyOfStates[finalStates[i]]].push_back(finalStatesMap[finalStates[i]]);
        }
    }
    for(int i=0; i<vertices.size(); i++)
    {
        if(keyOfStates.count(vertices[i]))
        {
            vertices.erase(vertices.begin()+i);
        }
    }
    int vertices_num= DFA.vertices.size() - keyOfStates.size();
    result.set_vertices(vertices);
    result.set_no_vertices(vertices_num);
    result.set_tran(transitions);
    return result;
}
