#ifndef SCRIPT_INTERPRETER_STEP_H_
#define SCRIPT_INTERPRETER_STEP_H_

#include <string>
#include <vector>
#include <map>

#define STEP_INDEX int

using namespace std;

//脚本应答逻辑的步骤。包含应答文本、用户输入时限、跳转到下一步骤的条件、无用户输入时的下一步骤、默认下一步骤
//note：下一步骤的索引STEP_INDEX定义来自于SyntaxTree
//example：
//    可以依据用户输入字符串获取下一步骤的STEP_INDEX
//    STEP_INDEX nextStep = getNextStep(answer);
class Step
{
public:
	//应答文本的元素
	struct Item
	{
		//标识字符串还是变量名
		bool isVarName;
		//字符串或变量名
		string str;
	};

	//应答文本表达式
	struct Expression
	{
		//应答文本的元素序列
		vector<Item> items;
	};

	//用户输入时限
	struct Listen
	{
		//开始时间
		int beginTimer;
		//结束时间
		int endTimer;
	};

	//跳转下一步骤的分支
	struct Branch
	{
		//输入条件字符串
		string answer;
		//下一步骤STEP_INDEX
		STEP_INDEX nextStep;
	};

	Step();

	inline const Expression& getSpeakExpression() const { return speak; }

	inline const Listen& getListen() const { return listenTimer; }

	inline void addExpressionItem(const bool isVarName, const string& str) 
	{ 
		speak.items.push_back({ isVarName, str }); 
	}

	inline void setListen(const int& beginTimer, const int& endTimer) 
	{ 
		listenTimer.beginTimer = beginTimer; 
		listenTimer.endTimer = endTimer;  
	}

	inline void addBranch(const string& answer, const STEP_INDEX& index)
	{
		branches.push_back({ answer, index });
	}

	inline void setSilence(const STEP_INDEX& index) { silence = index; }

	inline void setDefault(const STEP_INDEX& index) { deflt = index; }

	//依据用户意愿获取下一步骤的STEP_INDEX
	//parameter：
	//    const string& answer - 输入字符串
	//return:
	//    返回下一步骤的STEP_INDEX
	//    若字符串为空，返回无输入时的下一步骤
	//    若字符串不为空但没有匹配的下一步骤，返回默认下一步骤
	const STEP_INDEX getNextStep(const string& answer) const;

private:
	//应答文本
	Expression speak;
	//用户输入时限
	Listen listenTimer;
	//跳转下一步骤的所有分支, 序号在前的匹配优先级更高
	vector<Branch> branches;
	//无用户输入时下一步骤
	STEP_INDEX silence;
	//默认下一步骤
	STEP_INDEX deflt;
};

#endif  //SCRIPT_INTERPRETER_STEP_H_