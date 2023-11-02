#include "interpreter.h"

void Interpreter::output(const Step::Expression& exp)
{
	string str;
	for (Step::Item item : exp.items)
	{
		if (item.isVarName)
			str += varTable.find(item.str)->second;
		else
			str += item.str;
	}
	outputFunc(str);
}

//当前Step置为entryStep循环，针对当前Step做
//    输出应答文本
//    如果本步骤是终结步骤，则结束循环，断开通话
//    获取用户输入
//    获得下一个Step
void Interpreter::process()
{	
	STEP_INDEX currentStepId = kStTree->getEntry();
	while (true)
	{
		const Step& currentStep = kStTree->getStep(currentStepId);
		output(currentStep.getSpeakExpression());
		if (kStTree->isExit(currentStepId))
			break;
		string userInput;
		const Step::Listen& listenTimer = currentStep.getListen();
		input(&userInput, listenTimer.beginTimer, listenTimer.endTimer);
		currentStepId = currentStep.getNextStep(userInput);
	}
	exit();
}
