#include <iostream>
#include "recource.h"
#include "ClassFA.cpp"
#include "Functions.cpp"
#include "ReadInputFile.cpp"
#include "DFA.cpp"
#include "Minimization.cpp"
#include "OutputFile.cpp"

using namespace std;

int main()
{
    read_file("input.txt");
    FA result = language();
    FA dfa = NFAtoDFA(result);
    FA mini = minimizedTable(minimizaion(dfa), dfa);
    read_testProgram("test.txt", mini);
    return 0;
}
