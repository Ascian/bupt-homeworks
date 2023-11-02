#ifndef SCRIPT_INTERPRETER_SYNTAX_TREE_H_
#define SCRIPT_INTERPRETER_SYNTAX_TREE_H_

#include <set>
#include <vector>
#include <string>

#include "Step.h"
#include "../exception/script_exception.h"

using namespace std;

//脚本语法树。包含所有应答逻辑步骤，变量名，入口步骤和出口步骤
//example：
//    可以输入脚本文件路径，分析得到脚本语法树
//	  分析失败抛出异常，可以获取异常内容，分析脚本错误原因
//    try
//    {
// 		  syntaxTree.parse(scriptFileName);
//    }
//    catch(exception& e)
//    {
// 		  cout << e.what();
//    } 
class SyntaxTree
{
public:
	//输入脚本文件路径，解析脚本文件，得到脚本分析树
	//parameter：
	//    const string& scriptFilePath - 脚本文件路径
	//exception:
	//	  若文件打开错误，抛出ScriptFileException
	//    若脚本语法错误，抛出ScriptParseException
	void parse(const string& scriptFilePath);

	inline const STEP_INDEX& getEntry() const { return entry; }

	inline const bool isExit(const STEP_INDEX& id) const
	{
		if (exits.find(id) != exits.end())
			return true;
		else
			return false;
	}

	inline const Step& getStep(const STEP_INDEX& index) const { return steps[index]; }

	inline const vector<string>& getVarNames() const { return variables; }

private:
	//应答逻辑步骤
	vector<Step> steps;
	//变量名
	vector<string> variables;
	//入口步骤
	STEP_INDEX entry;
	//出口步骤
	set<STEP_INDEX> exits;
};

#endif  //SCRIPT_INTERPRETER_SYNTAX_TREE_H_