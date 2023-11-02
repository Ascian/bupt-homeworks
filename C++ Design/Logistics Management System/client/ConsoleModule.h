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
	vector<string> commands;//����
	vector<ConsoleModule*> subModules;//��ģ��
	vector<set<int>> ban;//��ģ���н�ֹʹ�õ��������
	int currentModule;//��ǰ��ģ��
public:
	ConsoleModule(const string& name) {
		this->name = name;
		currentModule = -1;
	}

	virtual ~ConsoleModule() {
		for (auto temp : subModules)
			delete(temp);
	}

	//��������
	virtual inline bool parse(SOCKET& cliSock, const string& command, const set<int>& ban) {
		if (currentModule != -1) {
			//������ģ��ִ��
			if (subModules.at(currentModule)->parse(cliSock, command, this->ban.at(currentModule)))
				currentModule = -1;
			return false;
		}
		return execute(cliSock, command, ban);
	}

	//ִ������
	virtual bool execute(SOCKET& cliSock, const string& command, const set<int>& ban) = 0;

	//��ȡ��ǰģ������
	virtual inline string getModuleName() {
		if (currentModule != -1) {
			return name + "$" + subModules.at(currentModule)->getModuleName();
		}
		return name;
	}

	//��ȡ��ǰģ������
	virtual vector<string> getModuleCommand(const set<int>& ban);
};