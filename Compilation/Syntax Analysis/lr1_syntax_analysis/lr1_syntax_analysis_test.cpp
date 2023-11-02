#include <iostream>
#include <fstream>
#include "lr1_syntax_analysis.h"

using namespace std;

int main()
{
    ifstream file;
    file.open("token_stream.txt", ios::in);

    syntax_analysis(file);

    file.close();
    return 0;
}
