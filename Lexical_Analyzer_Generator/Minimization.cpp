#include "Minimization.h"
#include "recource.h"
#include "ClassFA.h"
////////////////////////////////////////////////////////////////////////////////////////////
//
map<int,vector<string> > Mini;

/**-----------------------------------------------------------------------------------------
 * USEAGE : get Non_Final states from the difference between all the states and final states
 * Take   : all the states and final states
 * RETURN : Non_Final states
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
 * USEAGE : get inputs of all the transitions
 * Take   : transitions
 * RETURN : all possible inputs
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
 * USEAGE : get all the next state partitions with given inputs in a certain order
 * Take   : current state, transitions, inputs, map tells which partition next state is
 * RETURN : partitions where all the next state exist
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
 * USEAGE : Update the map that tells which partition next state is
 * Take   : number of new partition, element of that partition , map to update
 *-----------------------------------------------------------------------------------------*/

void updateMapValues(int value,vector<int> v, std::map<int, int> &keyOfStates)
{
    for(int i=0; i<v.size(); i++)
    {
        keyOfStates[v.at(i)] = value;
    }
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : collect FinalStates with the same category together
 * Take   : final states
 * RETURN : Related final states
 *-----------------------------------------------------------------------------------------*/
vector<vector<int>> separateFinalStates(vector<int> finalStates)
{
    map<int,int > collectRelater;
    vector<vector<int> > finalStatesRelated;
    for(int i = 0; i < finalStates.size(); i++)
    {
        if(!collectRelater.count(i))
        {
            vector<int> v;
            v.push_back(finalStates[i]);
            collectRelater[i] = 1;
            for(int j = 0; j < finalStates.size(); j++)
            {
                if(!collectRelater.count(j) && finalStatesMap[finalStates[i]] ==  finalStatesMap[finalStates[j]])
                {
                    v.push_back(finalStates[j]);
                    collectRelater[j] = 1;
                }
            }
            finalStatesRelated.push_back(v);
        }
    }
    return finalStatesRelated;
}
/**-----------------------------------------------------------------------------------------
 * USEAGE : initialize partitions, whichPartition and keyOfStates
 * Take   : whichPartition turn, partitions, Non_Final states,Related final States,
            map tells which partition next state is
 * RETURN : the last number assign to partition
 *-----------------------------------------------------------------------------------------*/
int  initial_values(queue<int> &whichPartition,map<int,vector<int> > &partitions,vector<int> &NonFinalStates, vector<vector<int>> &finalStatesRelated, std::map<int, int> &keyOfStates){
    int numberOfSets = 1;
    updateMapValues(numberOfSets,NonFinalStates,keyOfStates);
    partitions.insert(pair<int, vector<int> >(numberOfSets,NonFinalStates));
    whichPartition.push(numberOfSets);

    for(int i = 0; i < finalStatesRelated.size(); i++)
    {
        numberOfSets++;
        updateMapValues(numberOfSets,finalStatesRelated[i],keyOfStates);
        partitions.insert(pair<int, vector<int> >(numberOfSets,finalStatesRelated[i]));
        whichPartition.push(numberOfSets);
    }
    return numberOfSets;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : minimization process using partitioning method
 * Take   : DFA result
 * RETURN : final partitions that can be treated as one state
 *-----------------------------------------------------------------------------------------*/

map<int, vector<int> > minimizaion (FA DFA)
{
    int counter = 0;

    map<int, int> keyOfStates;
    map<string,vector<int> > pre_result;
    map<int,vector<int> > partitions;

    queue<int> whichPartition;

    vector<int> dividedPartitions;
    vector<int> finalStates = DFA.get_final_to_DFA();
    vector<int> NonFinalStates = getNonFinalStates(DFA.get_vertices(),finalStates);
    vector<int> currntVector = NonFinalStates;
    vector<transition> transitions = DFA.get_tran();
    vector<char> inputs = getInputs(transitions);
    vector<vector<int>> finalStatesRelated = separateFinalStates(finalStates);

    int numberOfSets = initial_values(whichPartition, partitions,NonFinalStates, finalStatesRelated, keyOfStates);

    while(currntVector.size() > counter)
    {
        string nextStates = findNextStats (currntVector[counter], transitions, inputs, keyOfStates);
        pre_result[nextStates].push_back(currntVector.at(counter));
        counter++;
        if(currntVector.size() <= counter)
        {
            if (pre_result.size() > 1)
            {
                for (std::map<string,vector<int> >::iterator it = pre_result.begin(); it!=pre_result.end(); ++it)
                {
                    numberOfSets++;
                    partitions.insert(pair<int,vector<int> >(numberOfSets, it->second));
                }
                dividedPartitions.push_back(whichPartition.front());
            }
            whichPartition.pop();
            if(!whichPartition.empty())
            {
                currntVector = partitions[whichPartition.front()];
            }
            else if(!dividedPartitions.empty())
            {
                for(int i = 0; i< dividedPartitions.size(); i++)
                    partitions.erase(dividedPartitions.at(i));

                dividedPartitions.clear();
                for (std::map<int,vector<int> >::iterator it = partitions.begin(); it!=partitions.end(); ++it)
                {
                    updateMapValues(it->first,it->second,keyOfStates);
                    whichPartition.push(it->first);
                }
                currntVector = partitions[whichPartition.front()];
            }
            else
            {
                break;
            }
            pre_result.clear();
            counter = 0;
        }
    }
    return partitions;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : make states diagram
 * Take   : minimization result and DFA result
 * RETURN : class of diagram contains vertices and transitions
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
        }
        else
        {
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
