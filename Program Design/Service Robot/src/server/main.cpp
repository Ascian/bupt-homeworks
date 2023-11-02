#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib") 
#include <Windows.h>

#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <mutex>

#include "lib/nlohmann/json.hpp"

#include "base/script_interpreter/syntax_tree.h"
#include "base/script_interpreter/interpreter.h"
#include "base/client_processor/client_processor.h"
#include "base/logger/logger.h"
#include "base/exception/socket_exception.h"
#include "base/exception/script_exception.h"

using namespace std;
using json = nlohmann::json;

//配置文件路径
const string kConfigPath = "config.json";

//服务器ip地址
string kServerIp;
//服务器端口号
int kServerPort;
//最大客户端连接数量
int kMaxConnections;
//最大消息队列长度
int kMaxBufSize;
//日志对象
Logger kLog;
//脚本语法树
SyntaxTree kSyntaxTree;
//客户端处理对象池
ClientProcessor* kProcessors;
//客户端处理对象分配信号量
HANDLE kProcessorNum;
//用户数据
json kUserData;

json* ClientProcessor::kUrData = &kUserData;
SyntaxTree* Interpreter::kStTree = &kSyntaxTree;

//读入配置文件，初始化程序
void initConfig();
//网络通信初始化
void initSocket(SOCKET& servSock);
//运行服务器
void serverWork(SOCKET& servSock);
//处理客户端请求
void processClient(ClientProcessor* processor);

int main()
{
    initConfig();
    SOCKET servSock;
    initSocket(servSock);

    serverWork(servSock);

    closesocket(servSock);
    WSACleanup();
    delete[] kProcessors;
    kLog.close();

    return 0;
}

//初始化脚本语法树
void initScript(const string& scriptFilePath)
{
    kLog.note("Begin parse script file \"" + scriptFilePath + "\"");
    try
    {
        kSyntaxTree.parse(scriptFilePath);  //分析脚本文件，得到脚本语法树
    }
    catch (const ScriptFileException& e)
    {
        kLog.error("ScriptFileException: " + string(e.what()));
        exit(-1);
    }
    catch (const ScriptParseException& e)
    {
        kLog.error("ScriptParseException: " + string(e.what()));
        exit(-1);
    }
    catch (const exception& e)
    {
        kLog.error(e.what());
        exit(-1);
    }
    kLog.note("Script parse succeeded");
}

//初始化客户端处理线程分配信号量
void initHandle()
{
    kProcessorNum = CreateSemaphore(NULL, kMaxConnections, kMaxConnections, NULL);  //初始值为最大连接数
    if (kProcessorNum == NULL)
    {
        kLog.error("Create semaphore failed");
        exit(-1);
    }
    kLog.note("Create semaphore succeeded, initial value: " + to_string(kMaxConnections));
}

void initConfig()
{ 
    ifstream f(kConfigPath);
    json config;
    try {
        config = json::parse(f);
        //初始化全局变量
        kServerIp = config["server_ip"].get<string>();
        kServerPort = config["server_port"].get<int>();
        kMaxConnections = config["max_connections"].get<int>();
        kMaxBufSize = config["max_buf_size"].get<int>();
        kLog.setup(config["log_dictionary_path"].get<string>(),
            config["max_generate_interval"].get<int>(),
            config["max_log_amount"].get<int>());
        initScript(config["script_file_path"].get<string>());
        kProcessors = new ClientProcessor[kMaxConnections];
        initHandle();
    }
    catch (exception& e)
    {
        kLog.error("ConfigParseException: " + string(e.what()));
        exit(-1);
    }

    ifstream uf(config["userdata_file_path"].get<string>());
    try
    {
        kUserData = json::parse(uf);
    }
    catch (exception& e)
    {
        kLog.error("UserdataParseException: " + string(e.what()));
        exit(-1);
    }
}

void initSocket(SOCKET& servSock)
{
    kLog.note("Begin init socket, IP: " + kServerIp + " port: " + to_string(kServerPort));
    WSADATA wsaData;
    int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0) {
        kLog.error("WSAStartup failed");
        exit(-1);
    }
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        WSACleanup();
        kLog.error("WSAStartup failed");
        exit(-1);
    }

    servSock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN servAdd;
    servAdd.sin_family = AF_INET;
    servAdd.sin_addr.S_un.S_addr = inet_addr(kServerIp.c_str());
    servAdd.sin_port = htons(kServerPort);
    if (::bind(servSock, (SOCKADDR*)&servAdd, sizeof(servAdd)) == SOCKET_ERROR) {
        kLog.error("Socket bind failed: " + to_string(WSAGetLastError()));
        WSACleanup();
        exit(-1);
    }
    if (listen(servSock, SOMAXCONN) == SOCKET_ERROR) {
        kLog.error("Socket listen failed: " + to_string(WSAGetLastError()));
        WSACleanup();
        exit(-1);
    }
    kLog.note("Socket init succeeded");
}

void serverWork(SOCKET& servSock)
{
    SOCKADDR_IN cliAdd;
    int len = sizeof(SOCKADDR);
    while (1) {
        SOCKET cliSock = accept(servSock, (SOCKADDR*)&cliAdd, &len);
        if (cliSock == SOCKET_ERROR) {  
            kLog.error("Accept client failed: " + to_string(WSAGetLastError()));
            break;
        }
        //等待可用客户端处理线程
        ClientProcessor* processor;
        WaitForSingleObject(kProcessorNum, INFINITE);
        for (int i = 0; i < kMaxConnections; i++) {
            if (kProcessors[i].getCliSock() == INVALID_SOCKET) {
                kProcessors[i].setCliSock(cliSock, cliAdd);
                processor = &kProcessors[i];
                break;
            }
        }
        //创建线程处理客户端请求
        thread th(&processClient, processor);
        th.detach();

        kLog.note("Accept client " + (string)inet_ntoa(cliAdd.sin_addr));
    }
}

void processClient(ClientProcessor* processor)
{
    SOCKADDR_IN cliAdd = processor->getCliAdd();
    try {
        kLog.note("(" + (string)inet_ntoa(cliAdd.sin_addr) + ") begin process");
        processor->process();
        kLog.note("(" + (string)inet_ntoa(cliAdd.sin_addr) + ") finish process");
    }
    catch (SocketDisconnectException& e)
    {
        kLog.error("SocketDisconnectException: " + string(e.what()));
    }
    catch (SocketErrorException& e)
    {
        kLog.error("SocketErrorException: " + string(e.what()));
    }
    catch (ScriptVariableNotExistException& e)
    {
        kLog.error("ScriptVariableNotExistException: " + string(e.what()));
    }
    catch (const exception& e)
    {
        kLog.error(e.what());
    }

    closesocket(processor->getCliSock());
    //设置SOCKET为INVALID_SOCKET，表示客户端处理对象空闲
    processor->setCliSock(INVALID_SOCKET, cliAdd);  
    ReleaseSemaphore(kProcessorNum, 1, NULL);  //释放一个信号量
    kLog.note("Disconnect client " + (string)inet_ntoa(cliAdd.sin_addr));
}