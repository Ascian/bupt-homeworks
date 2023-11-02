#ifndef SCRIPT_INTERPRETER_INTERPRETER_H_
#define SCRIPT_INTERPRETER_INTERPRETER_H_

#include <vector>
#include <map>
#include <string>
#include <functional>

#include "syntax_tree.h"

using namespace std;

//解释执行脚本语法树
//example:
//    首先需要指定脚本语法树
//    SyntaxTree* Interpreter::kStTree = &syntaxTree;
//    然后初始化解释器，提供输入输出,获取变量名对应的具体值和结束操作的回调函数
//    interpreter.init(inputFunc, outputFunc, exitFunc, getVarsFunc);
//    随后可以开始解释执行脚本语法树
//    interpreter.process();
class Interpreter
{
public:
	//初始化解释器
	//parameter:
	//    const function<void(string*, const int&, const int&)>& inputFunc - 输入回调函数
	//    const function<void(const string&)>& outputFunc - 输出回调函数
	//    const function<void()>& exitFunc - 结束操作回调函数
	//    const function<void(map<string, string>*, const vector<string>&)>& getVarsFunc - 获取变量名具体值的回调函数
	inline void init(const function<void(string*, const int&, const int&)>& inputFunc, 
		const function<void(const string&)>& outputFunc, const function<void()>& exitFunc, 
		const function<void(map<string, string>*, const vector<string>&)>& getVarsFunc)
	{
		this->outputFunc = outputFunc;
		this->inputFunc = inputFunc;
		this->exitFunc = exitFunc;
		getVarsFunc(&varTable, kStTree->getVarNames());
	}

	//解释执行脚本语法树
	void process();

private:
	//语法分析树
	static SyntaxTree* kStTree;
	//变量表，包含键值对{变量名, 变量值}
	map<string, string> varTable;
	//输入回调函数
	function<void(string*, const int&, const int&)> inputFunc;
	//输出回调函数
	function<void(const string&)> outputFunc;
	//结束操作回调函数
	function<void()> exitFunc;

	//输出应答文本
	//parameter:
	//    const Step::Expression& exp - 应答文本表达式
	void output(const Step::Expression& exp);

	//获取用户输入
	//parameter:
	//    string* str - 用户输入
	//	  const int& beginTimer - 开始输入时间
	//    const int& endTimer - 结束输入时间
	inline void input(string* str, const int& beginTimer, const int& endTimer) 
	{
		inputFunc(str, beginTimer, endTimer);
	}

	//执行结束
	inline void exit()
	{
		exitFunc();
	}
};

#endif  //SCRIPT_INTERPRETER_INTERPRETER_H_