#ifndef READINPUTFILE_H_INCLUDED
#define READINPUTFILE_H_INCLUDED
#include "recource.h"
#include "ClassFA.h"

void keyWords(string input);
void punc(string input);
FA return_FA(string symbols);
void expression_to_NFA(string name, string expression, string type);
void read_file (const char* input_file);
FA language();

extern map<string, FA> regular_Definitions;
extern map<string, FA> regular_Expression;
extern map<string, FA> key_words;
extern map<char, FA> punctuations;

#endif // READINPUTFILE_H_INCLUDED
