#include <iostream>
#include <string>
#include <regex>

#include "ll1_syntax_analysis.h"

using namespace std;

void input_grammar(Grammar& grammar)
{
    cout << "How many non-terminal symbols are there: ";
    string num;
    cin >> num;
    regex regx("^[0-9]*$");
    while (!regex_match(num, regx))
    {
        cout << "Invalid input" << endl;
        cout << "How many non-terminal symbols are there: ";
        cin >> num;
    }
    int n = stoi(num);
    cout << "non-terminal symbols: " << endl;
    for (int i = 1; i <= n; i++)
    {
        cout << "   [" << i << "] ";
        char symbol;
        cin >> symbol;
        grammar.add_non_terminal_symbol(symbol);
    }

    cout << "How many terminal symbols are there: ";
    cin >> num;
    while (!regex_match(num, regx))
    {
        cout << "Invalid input" << endl;
        cout << "How many terminal symbols are there: ";
        cin >> num;
    }
    n = stoi(num);
    cout << "terminal symbols: " << endl;
    for (int i = 1; i <= n; i++)
    {
        cout << "   [" << i << "] ";
        char symbol;
        cin >> symbol;
        if (!grammar.add_terminal_symbol(symbol))
        {
            cout << "The symbol is already defined as a non-terminal symbol" << endl;
            i--;
        }
    }

    cout << "How many productions are there: ";
    cin >> num;
    while (!regex_match(num, regx))
    {
        cout << "Invalid input" << endl;
        cout << "How many productions are there: ";
        cin >> num;
    }
    n = stoi(num);
    cout << "Note: the grammar productions should be left-recursion-eliminated" << endl;
    cout << "Note: if the right symbol string is epsilon, just input nothing and press the enter key" << endl;
    cout << "grammar productions: " << endl;
    for (int i = 1; i <= n; i++)
    {
        cout << "   [" << i << "] " << endl;
        cout << "       left: ";
        char symbol;
        cin >> symbol;
        cout << "       right: ";
        char str[100];
        cin.getline(str, 100);  //读取上次输入回车
        cin.getline(str, 100);
        if (!grammar.add_production(symbol, str))
        {
            cout << "The production contains unknown symbols" << endl;
            i--;
        }
    }

    cout << "initial non-terminal symbol: ";
    char symbol;
    cin >> symbol;
    while (!grammar.set_initial_symbol(symbol))
    {
        cout << "The symbol is not a non-terminal symbol" << endl;
        cout << "initial non-terminal symbol: ";
        cin >> symbol;
    }
}

int main()
{
    ifstream file;
    file.open("token_stream.txt", ios::in);
    Grammar grammar;

    input_grammar(grammar);   

    grammar.syntax_analysis(file);

    file.close();
    return 0;
}



