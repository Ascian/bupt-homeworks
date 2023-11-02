#pragma once
#include<string>
#include<vector>
#include<set>
#include<Winsock2.h>
#include<mutex>

#include"Logistics.h"
#include"Event.h"
#include"Command.h"

const int STRING_MAX_SIZE = 30;
const int MAX_BUFFER_SIZE = 5000;

using namespace std;

class Client {
public:
	SOCKET cliSock = INVALID_SOCKET;
	User* pUser = NULL;
	Courier* pCourier = NULL;
};

class ConsoleModule
{
protected:
	vector<ConsoleModule*> subModules;//��ģ��
	int currentModule;//��ǰ��ģ��
public:
	ConsoleModule() {
		currentModule = -1;
	}

	virtual ~ConsoleModule() {
		for (auto temp : subModules)
			delete(temp);
	}

	//��������
	virtual inline bool parse(Logistics* pLogistics, Client* pClient,const char* recvBuf, mutex& mutx) {
		if (currentModule != -1) {
			//������ģ��ִ��
			if (subModules.at(currentModule)->parse(pLogistics, pClient, recvBuf, mutx))
				currentModule = -1;
			return false;
		}
		return execute(pLogistics, pClient, recvBuf, mutx);
	}

	//ִ������
	virtual bool execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx) = 0;

    void display(Logistics* pLogistics, Client* pClient, const char kind, const char event);

	void search(Logistics* pLogistics, Client* pClient, const char kind, istringstream& recvInf, const char event);

	void findExpress(Logistics* pLogistics, Client* pClient, istringstream& recvInf);
};

