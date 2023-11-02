# ClientProcessor Class

与客户端进行网络通信，包含一个脚本解释器处理与用户交互逻辑

example:

    //首先指定用户数据来源
    json* ClientProcessor::kUserData = &userData;
    //然后接收到客户端请求后，设置SOCKET
    processor.setCliSock(cliSock, cliAdd);
    //随后开始与客户端进行网络通信
    processor.process();

- [Properties](#properties)
- [Public Functions](#public-functions)
  - [ClientProcessor()](#clientprocessor)
  - [const SOCKET\& getCliSock() const](#const-socket-getclisock-const)
  - [const SOCKADDR\_IN\& getCliAdd() const](#const-sockaddr_in-getcliadd-const)
  - [void setCliSock(const SOCKET\& cliSock, const SOCKADDR\_IN\& cliAdd)](#void-setclisockconst-socket-clisock-const-sockaddr_in-cliadd)
  - [void process()](#void-process)
- [Private Types](#private-types)
  - [enum Operation](#enum-operation)
- [Private Functions](#private-functions)
  - [const bool getVars(map\<string, string\>\* variables, const vector\& varNames, const string\& id)](#const-bool-getvarsmapstring-string-variables-const-vector-varnames-const-string-id)
  - [void getVarsFunc(map\<string, string\>\* variables, const vector\& varNames)](#void-getvarsfuncmapstring-string-variables-const-vector-varnames)
  - [void inputFunc(string\* str, const int\& beginTimer, const int\& endTimer)](#void-inputfuncstring-str-const-int-begintimer-const-int-endtimer)
  - [void outputFunc(const string\& str)](#void-outputfuncconst-string-str)
  - [void exitFunc()](#void-exitfunc)



---
## Properties

|Type|Name|Description|
|---|---|---|
|static json*|kUserData|用户数据源|
|SOCKET|cliSock|客户端通信SOCKET，为`INVALID_SOCKET`，表示该对象空闲|
|SOCKADDR_IN|cliAdd|客户端地址|
|Interpreter|interpreter|脚本解释器|

---
## Public Functions

|Return|Statement|Description|
|---|---|---|
||ClientProcessor()||
|const SOCKET&|getCliSock() const|获取客户端SOCKET|
|const SOCKADDR_IN&|getCliAdd() const|获取客户端地址信息|
|void|setCliSock(const SOCKET& cliSock, const SOCKADDR_IN& cliAdd)|设置客户端SOCKET和客户端地址信息|
|void|process()|与客户端进行网络通信，解释执行脚本文件，处理与用户交互逻辑|

---
### ClientProcessor()

初始化客户端处理对象，初始化`cliSock`为INVALID_SOCKET，表示该对象空闲

---
### const SOCKET& getCliSock() const

获取客户端SOCKET

---
### const SOCKADDR_IN& getCliAdd() const

获取客户端地址信息

---
### void setCliSock(const SOCKET& cliSock, const SOCKADDR_IN& cliAdd)

设置客户端SOCKET和客户端地址信息

---
### void process()

调用脚本解释模块，解释执行脚本文件。将脚本解释器提交的输入输出请求，转变为与客户端进行网络通信的输入输出。

---
## Private Types

|Type|Name|
|---|---|
|enum|Operation|

---
### enum Operation

指示客户端执行的操作类型

|Name|Description|
|---|---|
|EXIT|通信结束|
|SPEAK|输出应答文本|
|LISTEN|获取用户输入|

---
## Private Functions

|Return|Statement|Description|
|---|---|---|
|const bool|getVars(map<string, string>* variables, const vector<string>& varNames, const string& id)|依据用户id获取用户数据|
|void|getVarsFunc(map<string, string>* variables, const vector<string>& varNames)|接受用户id，获取用户数据|
|void|inputFunc(string* str, const int& beginTimer, const int& endTimer)|接受用户输入|
|void|outputFunc(const string& str)|发送应答文本|
|void|exitFunc()|发送通信结束消息|

---
### const bool getVars(map<string, string>* variables, const vector<string>& varNames, const string& id)

依据用户id和变量名表获取用户对应的变量数据

#### parameter

* map<string, string>* variables - 变量表，包含键值对{变量名, 变量值}
* const vector<string>& varNames - 变量名
* const string& id - 用户id

#### return

返回用户id是否存在

#### exception

* 若变量名对应的变量不存在用户数据中，抛出ScriptVariableNotExistException

---
### void getVarsFunc(map<string, string>* variables, const vector<string>& varNames)

接受用户id，获取用户变量数据

#### parameter

* map<string, string>* variables - 变量表，包含键值对{变量名, 变量值}
* const vector<string>& varNames - 变量名

#### exception

* 若变量名对应的变量不存在用户数据中，抛出ScriptVariableNotExistException
* 若网络连接断开，抛出SocketDisconnectException
* 若网络连接出错，抛出SocketErrorException

---
### void inputFunc(string* str, const int& beginTimer, const int& endTimer)

在设置的时间范围内，接受客户端用户输入

#### parameter

* string* str - 用户输入
* const int& beginTimer - 输入开始时间
* const int& endTimer - 输入结束时间

#### exception

* 若网络连接断开，抛出SocketDisconnectException
*  若网络连接出错，抛出SocketErrorException

---
### void outputFunc(const string& str)

发送应答文本到客户端

#### parameter

* const string& str - 应答文本

#### exception

* 若网络连接断开，抛出SocketDisconnectException
*  若网络连接出错，抛出SocketErrorException

---
### void exitFunc()

发送通信结束消息到客户端