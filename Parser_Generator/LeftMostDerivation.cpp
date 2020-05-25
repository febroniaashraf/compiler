#include "leftMostDerivation.h"
#include "First.h"
#include "recource.h"

/**----------------------------------------------------------------------------------------
 * USEAGE : This Function reads output file from Phase 1 as an Input for LeftMostDerivation
 * RETURN : Queue of string contains The lexical analyzer
 *-----------------------------------------------------------------------------------------*/

queue<string> readOutAsIn()
{
    string line;
    queue<string> queueInputs;
    string file_path = __FILE__;
    string dir_path = file_path.substr(0, file_path.rfind("\\"));
    string dir_path2 = dir_path.substr(0, dir_path.rfind("\\"));
    ifstream myfile (dir_path2+"\\output_phase1.txt");
    regex regexX("(.*)( --> )(.*)"); // regex according to Output file of phase1 format
    smatch match;
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            if (regex_search(line, match, regexX) == true)
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
    queueInputs.push("$"); // last element in input queue is $
    return queueInputs;
}

/**----------------------------------------------------------------------------------------
 * USEAGE : LeftMostDerivation process happens here with Panic mode recovery
 * TAKE   : Predictive parsing table for the given grammar
 * RETURN : Vector represent Tracing of moves made by predictive parser for certain Input
 *-----------------------------------------------------------------------------------------*/

vector<string> leftMostDerivation(map<string, map<string, vector<string> > > table)
{
    vector<string> result;
    std::queue<std::string> queueInputs = readOutAsIn();
    stack<string> stackProc;
    // first element in stack is $
    stackProc.push("$");
    // second element in stack is the start non_terminal
    stackProc.push(all_nonTerminals.at(0).name);
    while(queueInputs.front() != "$")
    {
        if(is_terminal(stackProc.top()))
        {
            // case 1: if top of stack is terminal that equals first element in input
            if(stackProc.top() == queueInputs.front())
            {
                result.push_back("accept " + stackProc.top());
                stackProc.pop();
                queueInputs.pop();
            }
            // case 2: if top of stack is $ and input hasn't finished yet
            else if(stackProc.top() == "$")
            {
                result.push_back("error: can't complete");//this make error can't be recovered
                break;
            }
            // case 3: if top of stack is terminal that not equals first element in input
            else
            {
                result.push_back("error: missing " + stackProc.top() + " inserted");
                stackProc.pop();
            }
        }
        else
        {
            vector<string> ss = table[stackProc.top()][queueInputs.front()];
            if(ss.size() != 0)// check table cell is if not empty
            {
                // case 1: if cell value is epsilon
                if(ss.at(0) == "^")
                {
                    stackProc.pop();
                }
                // case 2: if cell value is synch
                else if(ss.at(0) == "synch")
                {
                    result.push_back("error happens here");
                    stackProc.pop();
                }
                // case 3: cell has Terminals or non_Terminals Values
                else {
                // replace the non_terminal value of the top of stack with cell value
                string s = stackProc.top() + " --> ";
                stackProc.pop();
                for(int i = ss.size() - 1; i >= 0; i--)
                {
                    s = s + ss.at(i) + " ";
                    stackProc.push(ss.at(i));
                }
                result.push_back(s);
                }
            }
            else // if the cell is empty
            {
                result.push_back("error: illegal " + stackProc.top() + " discord "  + queueInputs.front());
                queueInputs.pop();
            }
        }
    }
    while(stackProc.top() != "$") // stack still has values
    {
        if(is_terminal(stackProc.top()))
        {
            result.push_back(stackProc.top()+ " missing ");
        }
        stackProc.pop();
    }
    return result;
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : Write the result(predictive parsing table & LeftMostDerivation output) in a file
 * TAKE   : predictive parsing table & LeftMostDerivation output
 *------------------------------------------------------------------------------------------*/

void outputFile(vector<string> leftMost,map<string, map<string, vector<string> > > table)
{
    std::ofstream outfile;
    string file_path = __FILE__;
    string dir_path = file_path.substr(0, file_path.rfind("\\"));
    string dir_path2 = dir_path.substr(0, dir_path.rfind("\\"));
    outfile.open(dir_path2+"\\output_phase2.txt", std::ios_base::trunc); // overwrite
    outfile << "*********************************************************************************************" << "\n";
    outfile << "The predictive parsing table : \n";
    outfile << "*********************************************************************************************" << "\n";
    //write predictive parsing table
    map<string, map<string, vector<string>>>::iterator it1;
    map<string, vector<string>>::iterator it2;
    for(it1 = table.begin(); it1 != table.end(); ++it1)
    {
        outfile << "Non terminal : " << it1->first << " --> \n";
        map<string, vector<string>> s = it1->second;
        for (it2 = s.begin(); it2 != s.end(); ++it2)
        {
            string str = it2->first;
            vector<string> str2 = it2->second;
            outfile << "If " << str << " then ";
            for(int i = 0; i < str2.size(); i++)
            {
                outfile << str2.at(i)<< " ";
            }
            outfile << "\n";
        }
        outfile << "\n";
        outfile << "-----------------------------------------------------------------------------------------" << "\n";
    }
    //write LeftMostDerivation output
    outfile << "*********************************************************************************************" << "\n";
    outfile << "Left Most Derivation Output : \n";
    outfile << "*********************************************************************************************" << "\n";
    for(int i = 0; i < leftMost.size(); i++)
    {
        outfile << leftMost.at(i) << "\n";
        outfile << "-----------------------------------------------------------------------------------------" << "\n";
    }
    outfile.close();
}