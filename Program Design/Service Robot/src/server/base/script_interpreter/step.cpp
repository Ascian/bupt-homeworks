#include "step.h"

Step::Step()
{
	listenTimer = {5, 20};
	silence = 0;
	deflt = 0;
}

//简单匹配用户输入字符串和跳转条件，序号在前的branch优先匹配
const STEP_INDEX Step::getNextStep(const string& str) const
{
	if(str.size() == 0)
		return silence;
	for (Branch branch : branches)
	{
		if (str.find(branch.answer) != string::npos)
			return branch.nextStep;
	}
	return deflt;
}
