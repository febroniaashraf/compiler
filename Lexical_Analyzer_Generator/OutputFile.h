#ifndef OUTPUTFILE_H_INCLUDED
#define OUTPUTFILE_H_INCLUDED
#include "recource.h"
#include "ClassFA.h"

string construct_output(string word, FA mini);
void read_testProgram(const char* input_file, FA mini);
void writeOutput(vector<pair<string,string>>);

extern map<string, FA> regular_Expression;
extern map<string, FA> key_words;
extern map<char, FA> punctuations;
extern map<int,vector<string> > Mini;
#endif // OUTPUTFILE_H_INCLUDED
