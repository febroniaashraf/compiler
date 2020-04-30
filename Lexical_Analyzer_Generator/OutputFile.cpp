#include "OutputFile.h"
#include "recource.h"
#include "ClassFA.h"

/**-----------------------------------------------------------------------------------------
 * USEAGE : checks if input word is included in language then write in output file
 * Take   : word that is checked and minimal FA of language
 *-----------------------------------------------------------------------------------------*/

void construct_output(string word, FA mini)
{
    vector<transition> trans = mini.get_tran();
    vector<int> final_states = mini.get_final_to_DFA();
    vector<int> fin;
    vector<int> v;
    for(int i=0;i<word.length();i++)
    {
        bool flag = false;
        vector<int> v1;
        if(i==0)
        {
            v.push_back(mini.get_startState());
        }
        for(int k = 0;k<v.size();k++)
        {
          for(int j=0;j<trans.size();j++)
          {
              transition tran = trans.at(j);
              if(tran.vertex_from == v.at(k))
              {
                  if(tran.symbol == word[i])
                  {
                      v1.push_back(tran.vertex_to);
                      flag = true;
                  }
              }
          }
        }
        v.clear();
        for(int k =0;k<v1.size();k++)
        {
            v.push_back(v1.at(k));
        }
        v1.clear();
    }
    for(int i=0;i<v.size();i++)
    {
        map<int,vector<string> >::iterator itr;
        itr = Mini.find(v.at(i));
        if(itr != Mini.end())
        {
            fin.push_back(v.at(i));
        }
    }
    string output = "";
    string output1 = "";
    string output2 = "";
    string output3 = "";
    if(fin.size()==0)
    {
        output += word+" --> Error, Not Included In Language.";
    }
    for(int i=0;i<fin.size();i++)
    {
                map<int, vector<string> >::iterator itr;
                itr = Mini.find(fin.at(i));
                if(itr != Mini.end())
                {
                for(int j=0;j<itr->second.size();j++)
                {
                map<string,FA>::iterator it;
                it = key_words.find(itr->second.at(j)); // check if symbol exists in Re DE map
                if(it != key_words.end() && word == itr->second.at(j))
                {
                    output1 = itr->second.at(j);
                }
                it = regular_Expression.find(itr->second.at(j));
                if(it != regular_Expression.end())
                {
                    output2 = itr->second.at(j);
                }
                if(word == itr->second.at(j) && output1== "")
                {
                    output3 = itr->second.at(j);
                }
                }
            }
    }
    if(output1 != "")
    {
        output = output1;
    }
    else if(output3 != "")
    {
        output = output3;
    }
    else
    {
        output = output2;
    }
    std::ofstream outfile;
    string file_path = __FILE__;
    string dir_path = file_path.substr(0, file_path.rfind("\\"));
    string dir_path2 = dir_path.substr(0, dir_path.rfind("\\"));
    outfile.open(dir_path2+"\\output_phase1.txt", std::ios_base::app); // append instead of overwrite
    outfile << word+" --> "+output+"\n";
    outfile.close();
}

/**-----------------------------------------------------------------------------------------
 * USEAGE : read test program file to build output file
 * Take   : name of test file and minimal FA of language
 *-----------------------------------------------------------------------------------------*/

void read_testProgram(const char* input_file, FA mini)
{
    fstream newfile;
    newfile.open(input_file,ios::in); //open a file to perform read operation using file object
    if (newfile.is_open())    //checking whether the file is open
    {
        string line = "";
        while(getline(newfile, line))  //read data from file object and put it into string.
        {
            string word = "";
            for (std::string::size_type i = 0; i < line.size(); i++)
            {
                if(line[i] == ' ')
                {
                    if(word != "")
                    {
                      construct_output(word, mini);
                      word = "";
                    }
                }
                else
                {
                    word+= line[i];
                }
            }
            if(word != " " && word != "")
            {
                construct_output(word, mini);
                word = "";
            }
        }
        newfile.close(); //close the file object.
    }
}
