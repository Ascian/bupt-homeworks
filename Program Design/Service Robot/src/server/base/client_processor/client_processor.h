#ifndef CLIENT_PROCESSOR_H_
#define CLIENT_PROCESSOR_H_

#include <Winsock2.h>
#pragma comment(lib,"ws2_32.lib") 

#include <vector>
#include <map>
#include <string>
#include <functional>

#include "../../lib/nlohmann/json.hpp"

#include "../script_interpreter/interpreter.h"
#include "../exception/socket_exception.h"
#include "../exception/script_exception.h"

using namespace std;
using json = nlohmann::json;

//与客户端进行网络通信，包含一个脚本解释器处理与用户交互逻辑
//example:
//    首先指定用户数据来源
//    json* ClientProcessor::kUrData = &userData;
//    然后接收到客户端请求后，设置SOCKET
//    processor.setCliSock(cliSock, cliAdd);
//    随后开始与客户端进行网络通信
//    processor.process();
class ClientProcessor
{
public:
	ClientProcessor()
	{
		cliSock = INVALID_SOCKET;
	}

	inline const SOCKET& getCliSock() const { return cliSock; }

	inline const SOCKADDR_IN& getCliAdd() const { return cliAdd; }

	inline void setCliSock(const SOCKET& cliSock, const SOCKADDR_IN& cliAdd)
	{
		this->cliSock = cliSock;
		this->cliAdd = cliAdd;
	}

	//与客户端进行网络通信，解释执行脚本文件，处理与用户交互逻辑
	void process();

private:
	//指示客户端执行的操作类型
	enum Operation
	{
		//通信结束
		EXIT,
		//输出应答文本
		SPEAK,
		//获取用户输入
		LISTEN,
	};
	
	//用户数据源
	static json* kUrData;
	//客户端通信SOCKET，为INVALID_SOCKET，表示该对象空闲
	SOCKET cliSock;
	//客户端地址
	SOCKADDR_IN cliAdd;
	//脚本解释器
	Interpreter interpreter;

	//依据用户id获取用户数据
	//parameter:
	//    map<string, string>* variables - 变量表，包含键值对{变量名, 变量值}
	//    const vector<string>& varNames - 变量名
	//    const string& id - 用户id
	//return:
	//    返回用户id是否存在
	//exception:
	//    若变量名对应的变量不存在用户数据中，抛出ScriptVariableNotExistException
	const bool getVars(map<string, string>* variables, const vector<string>& varNames, 
		const string& id);

	//接受用户id，获取用户数据
	//parameter:
	//    map<string, string>* variables - 变量表，包含键值对{变量名, 变量值}
	//    const vector<string>& varNames - 变量名
	//exception:
	//    若变量名对应的变量不存在用户数据中，抛出ScriptVariableNotExistException
	//    若网络连接断开，抛出SocketDisconnectException
	//    若网络连接出错，抛出SocketErrorException
	void getVarsFunc(map<string, string>* variables, const vector<string>& varNames);

	//接受用户输入
	//parameter:
	//    string* str - 用户输入
	//    const int& beginTimer - 输入开始时间
	//    const int& endTimer - 输入结束时间
	//exception:
	//    若网络连接断开，抛出SocketDisconnectException
	//    若网络连接出错，抛出SocketErrorException
	void inputFunc(string* str, const int& beginTimer, const int& endTimer);

    //发送应答文本
	//parameter:
	//    const string& str - 应答文本
	//exception:
	//    若网络连接断开，抛出SocketDisconnectException
	//    若网络连接出错，抛出SocketErrorException
	void outputFunc(const string& str);

    //发送通信结束消息
	inline void exitFunc()
	{
		// |Exit(1)| -> client
		char exit = EXIT;
		send(cliSock, &exit, 1, 0);
	}
};

#endif  //CLIENT_PROCESSOR_H_