#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <map>

#include "client.h"
#include "../base/script_interpreter/syntax_tree.h"
#include "../base/script_interpreter/interpreter.h"
#include "../base/exception/script_exception.h"

SyntaxTree syntaxTree;
SyntaxTree* Interpreter::kStTree = &syntaxTree;

using namespace std;

void getVariable(int num, char** str, map<string, string>* vars, const vector<string>& varNames);

int main(int argc, char* argv[])
{
    if(argc < 2)
        return 0;
    //脚本解析测试 
    try
    {
        syntaxTree.parse(argv[1]);
    }
    catch (const ScriptParseException& e)
    {
        cout << e.what() << endl;
        return 0;
    }
    cout << "success" << endl;
    //脚本解释执行测试
    if (argc > 2)
    {
        Client client(argv[2]);  //客户端模拟测试桩
        function<void(map<string, string>*, const vector<string>&)> getVarsFunc
            = bind(&getVariable, argc - 3, (char**)argv + 3, placeholders::_1, placeholders::_2);
        function<void()> exitFunc = []() {};
        Interpreter interpreter;
        interpreter.init(client.getInputFunc(), client.getOuputFunc(),
            exitFunc, getVarsFunc);
        interpreter.process();
    }

    return 0;
}

void getVariable(int num, char** str, map<string, string>* vars, const vector<string>& varNames)
{
    string varName;
    string varValue;
    for (int i = 0; i < num; i++)
    {
        int j;
        string varExpression = str[i];
        //str格式为 varName=varValue
        for (j = 0; j < varExpression.size(); j++)
        {
            if (varExpression[j] == '=')
                break;
        }
        varName.assign(varExpression, 0, j);
        varValue.assign(varExpression, j + 1, varExpression.size() - j - 1);
        vars->insert({ varName, varValue });
    }
}