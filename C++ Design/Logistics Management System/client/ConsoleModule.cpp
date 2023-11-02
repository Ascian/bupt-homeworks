#include "ConsoleModule.h"

vector<string> ConsoleModule::getModuleCommand(const set<int>& ban)
{
	if (currentModule != -1)
		return subModules.at(currentModule)->getModuleCommand(this->ban.at(currentModule));
	vector<string> result;
	for (int i = 0; i < commands.size(); i++)
		if (!ban.contains(i))
			result.push_back(commands.at(i));
	return result;
}
