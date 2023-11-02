
#include "CFG.h"
#include <regex>

using namespace std;

void inputGramma(CFG& cfg);

int main()
{
    CFG cfg;
    inputGramma(cfg);
    

    cout << "The equivalent grammar with no ε production, no single production and no not_used symbol:" << endl;
    try {
        cfg.removeDeltaTrans();
        cfg.removeSingleTrans();
        cfg.removeNoUseSymb();
        cout << cfg << endl;
    }
    catch (const char* msg) {
        cout << msg;
        return 0;
    }

    cout << "Show equivalent CNF(Y/N): ";
    string isShow;
    cin >> isShow;
    while (isShow != "Y" && isShow != "y" && isShow != "N" && isShow != "n") {
        cout << "Invalid input" << endl;
        cout << "Show equivalent CNF(Y/N): ";
        cin >> isShow;
    }
    if (isShow == "Y" || isShow == "y") {
        try {
            cfg.transformToCNF();
            cout << cfg << endl;
        }
        catch (const char* msg) {
            cout << msg;
            return 0;
        }
    }
    else
        return 0;
    
    cout << "Show equivalent GNF(Y/N): ";
    cin >> isShow;
    while (isShow != "Y" && isShow != "y" && isShow != "N" && isShow != "n") {
        cout << "Invalid input" << endl;
        cout << "Show equivalent GNF(Y/N): ";
        cin >> isShow;
    }
    if (isShow == "Y" || isShow == "y") {
        try {
            cfg.transformToGNF();
            cout << cfg << endl;
        }
        catch (const char* msg) {
            cout << msg;
        }
    } 

    system("pause");
    return 0;
}

void inputGramma(CFG& cfg) {
    cout << "How many non-terminal symbols are there in the gramma: ";
    string num;
    cin >> num;
    regex regx("^[1-9]\d*|0$");
    while (!regex_match(num, regx)) {
        cout << "Invalid input" << endl;
        cout << "How many non-terminal symbols are there in the gramma: ";
        cin >> num;
    }
    int n = stoi(num);
    cout << "Non-terminal symbols: " << endl;
    for (int i = 1; i <= n; i++) {
        cout << "   [" << i << "] ";
        char symbol;
        cin >> symbol;
        try {
            cfg.addN(symbol);
        }
        catch (const char* msg) {
            cout << msg << endl;
            i--;
        }
    }

    cout << "How many terminal symbols are there in the gramma: ";
    cin >> num;
    while (!regex_match(num, regx)) {
        cout << "Invalid input" << endl;
        cout << "How many terminal symbols are there in the gramma: ";
        cin >> num;
    }
    n = stoi(num);
    cout << "Terminal symbols: " << endl;
    for (int i = 1; i <= n; i++) {
        cout << "   [" << i << "] ";
        char symbol;
        cin >> symbol;
        try {
            cfg.addT(symbol);
        }
        catch (const char* msg) {
            cout << msg << endl;
            i--;
        }
    }

    cout << "How many productions are there in the gramma: ";
    cin >> num;
    while (!regex_match(num, regx)) {
        cout << "Invalid input" << endl;
        cout << "How many productions are there in the gramma: ";
        cin >> num;
    }
    n = stoi(num);
    cout << "Grammar production:" << endl;
    for (int i = 1; i <= n; i++) {
        cout << "   [" << i << "] " << endl;
        cout << "      left: ";
        char symbol;
        cin >> symbol;
        cout << "      right: ";
        char str[100];
        cin.getline(str, 100);
        cin.getline(str, 100);
        try {
            cfg.addP(symbol, str);
        }
        catch (const char* msg) {
            cout << msg << endl;
            i--;
        }     
    }

    cout << "Initial non-terminal symbol: ";
    char symbol;
    cin >> symbol;
    cfg.setInitSymb(symbol);
}
