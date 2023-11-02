#include <iostream>
#include <fstream>
#include "recursive_syntax_analysis.h"

using namespace std;

int main()
{
    ifstream file;
    file.open("token_stream.txt", ios::in);

    syntax_analysis(file);

    file.close();
    return 0;
}
