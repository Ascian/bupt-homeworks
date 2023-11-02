#include "client_processor.h"

//缓冲区最大大小
extern int kMaxBufSize;

void ClientProcessor::process()
{
	function<void(map<string, string>*, const vector<string>&)> getVarsFunc =
		bind(&ClientProcessor::getVarsFunc, this, placeholders::_1,
		placeholders::_2);

	function<void(const string&)> outputFunc = 
		bind(&ClientProcessor::outputFunc, this, placeholders::_1);

	function<void(string*, const int&, const int&)> inputFunc = 	
		bind(&ClientProcessor::inputFunc, this, placeholders::_1, 
		placeholders::_2, placeholders::_3);

	function<void()> exitFunc = bind(&ClientProcessor::exitFunc, this);
	//初始化脚本解释器，传入用于输入输出、执行结束操作和获取用户数据的回调函数
	interpreter.init(inputFunc, outputFunc, exitFunc, getVarsFunc);  
	interpreter.process();  //执行脚本解释器
}

const bool ClientProcessor::getVars(map<string, string>* variables, const vector<string>& varNames, const string& id)
{
	if (!kUrData->contains(id))
		return false;
	//在用户数据中寻找用户id和变量名对应的变量值，存储为string类型
	for (string VarName : varNames)
	{
		if (!kUrData->at(id).contains(VarName))
			throw ScriptVariableNotExistException(VarName);
		if((*kUrData)[id][VarName].is_string())
			variables->insert({ VarName, (*kUrData)[id][VarName]});
		else
			variables->insert({ VarName, (*kUrData)[id][VarName].dump()});
	}
	return true;
}

void ClientProcessor::getVarsFunc(map<string, string>* variables, const vector<string>& varNames)
{
	// |id(1~kMaxRecvBufSize)| <- client
	char* buf = new char[kMaxBufSize];
	int len = recv(cliSock, buf, kMaxBufSize, 0);
	if (len == 0)
	{
		delete[] buf;
		throw SocketDisconnectException(inet_ntoa(cliAdd.sin_addr));
	}
	if (len < 0)
	{
		delete[] buf;
		throw SocketErrorException(WSAGetLastError());
	}

	string id(buf, len);
	while (!getVars(variables, varNames, id))
	{
		//用户名不存在
		// |false(1)| -> client
		char f = false;
		send(cliSock, &f, 1, 0);
		// |id(0~kMaxBufSize)| <- client
		int len = recv(cliSock, buf, kMaxBufSize, 0);
		if (len == 0)
		{
			delete[] buf;
			throw SocketDisconnectException(inet_ntoa(cliAdd.sin_addr));
		}
		if (len < 0)
		{
			delete[] buf;
			throw SocketErrorException(WSAGetLastError());
		}
		id.assign(buf, len);
	}
	// |true(1)| -> client
	char t = true;
	send(cliSock, &t, 1, 0);
	delete[] buf;
}

void ClientProcessor::inputFunc(string* str, const int& beginTimer, const int& endTimer)
{
	// |LISTEN(1)|beginTimer(4)|endTimer(4)| -> client
	char outputBuf[9];
	outputBuf[0] = LISTEN;
	memcpy(outputBuf + 1, &beginTimer, 4);
	memcpy(outputBuf + 5, &endTimer, 4);
	send(cliSock, outputBuf, 9, 0);

	// |isEmpty(1)|text(0~(kMaxBufSize-1))| <- client
	char* inputBuf = new char[kMaxBufSize]; 
	int len = recv(cliSock, inputBuf, kMaxBufSize, 0);
	if (len == 0)
	{
		delete[] inputBuf;
		throw SocketDisconnectException(inet_ntoa(cliAdd.sin_addr));
	}
	if (len < 0)
	{
		delete[] inputBuf;
		throw SocketErrorException(WSAGetLastError());
	}
	if (inputBuf[0])
		str->assign(inputBuf + 1, len - 1);
	else
		str->clear();
	delete[] inputBuf;
}

void ClientProcessor::outputFunc(const string& str)
{
	// |SPEAK(1)|text(0~(kMaxBufSize-1))| -> client
	char* buf = new char[kMaxBufSize];
	buf[0] = SPEAK;
	int len = kMaxBufSize;
	if (str.size() < kMaxBufSize - 1)
		len = str.size() + 1;
	memcpy(buf + 1, str.c_str(), len - 1);
	send(cliSock, buf, len, 0);
	delete[] buf;

	// |ACK(1)| <- client
	char ack;
	len = recv(cliSock, &ack, 1, 0);
	if (len == 0)
		throw SocketDisconnectException(inet_ntoa(cliAdd.sin_addr));
	if (len < 0)
		throw SocketErrorException(WSAGetLastError());
}
