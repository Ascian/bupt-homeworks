#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<regex>
#include<climits>
#include<set>
#include<Winsock2.h>
#include<sstream>
#include<iomanip>

#include"Event.h"
#include"Command.h"

const int STRING_MAX_SIZE = 30;
const int MAX_BUFFER_SIZE = 5000;

using namespace std;

class ConsoleModule
{
protected:
	string name;
	vector<string> commands;//命令
	vector<ConsoleModule*> subModules;//子模块
	vector<set<int>> ban;//子模块中禁止使用的命令序号
	int currentModule;//当前子模块
public:
	ConsoleModule(const string& name) {
		this->name = name;
		currentModule = -1;
	}

	virtual ~ConsoleModule() {
		for (auto temp : subModules)
			delete(temp);
	}

	//分析命令
	virtual inline bool parse(SOCKET& cliSock, const string& command, const set<int>& ban) {
		if (currentModule != -1) {
			//交给子模块执行
			if (subModules.at(currentModule)->parse(cliSock, command, this->ban.at(currentModule)))
				currentModule = -1;
			return false;
		}
		return execute(cliSock, command, ban);
	}

	//执行命令
	virtual bool execute(SOCKET& cliSock, const string& command, const set<int>& ban) = 0;

	//获取当前模块名字
	virtual inline string getModuleName() {
		if (currentModule != -1) {
			return name + "$" + subModules.at(currentModule)->getModuleName();
		}
		return name;
	}

	//获取当前模块命令
	virtual vector<string> getModuleCommand(const set<int>& ban);
};