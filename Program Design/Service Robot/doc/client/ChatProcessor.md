# ChatProcessor Class : QObject

Inherits: [QObject](https://doc.qt.io/qt-6/qobject.html)

处理与服务器通信

example:

    //处理用户登录，向服务器发送用户登录请求
    chatProcessor.login(userId);
    //与服务器开始“聊天”，处理服务器发送的任务
    chatProcessor.chat();

- [Properties](#properties)
- [Public Functions](#public-functions)
  - [ChatProcessor()](#chatprocessor)
  - [~ChatProcessor()](#chatprocessor-1)
  - [void addInputBuf(const QString\& input)](#void-addinputbufconst-qstring-input)
  - [void close()](#void-close)
- [Signals:](#signals)
  - [void loginSucceed()](#void-loginsucceed)
  - [void loginFail(const QString\& err)](#void-loginfailconst-qstring-err)
  - [void forbidInput()](#void-forbidinput)
  - [void allowInput()](#void-allowinput)
  - [void serverOutput(const QString\& str)](#void-serveroutputconst-qstring-str)
  - [void error(const QString\& err)](#void-errorconst-qstring-err)
  - [void end()](#void-end)
- [Public Slots](#public-slots)
  - [void chat()](#void-chat)
  - [void login(const QString\& id)](#void-loginconst-qstring-id)
- [Private Types](#private-types)
  - [enum Operation](#enum-operation)
- [Private Functions](#private-functions)
  - [void initSocket()](#void-initsocket)
  - [void sendInputBuf()](#void-sendinputbuf)


---
## Properties

|Type|Name|Description|
|---|---|---|
|SOCKET|cliSock|与服务器通信的SOCKET|
|QString|inputBuf|用户输入缓冲区|
|bool|isClose|是否强制停止工作|

---
## Public Functions

|Return|Statement|Description|
|---|---|---|
||ChatProcessor()||
||~ChatProcessor()||
|void|addInputBuf(const QString& input)|添加用户输入到缓冲区|
|void|close()|强制停止工作|

---
### ChatProcessor()

构建一个处理与服务器通信的对象，初始化SOCKET

---
### ~ChatProcessor()

析构处理与服务器通信的对象，关闭SOCKET

---
### void addInputBuf(const QString& input)

添加用户输入到缓冲区

#### parameter

* const QString& input - 用户输入

---
### void close()

强制对象与服务器停止通信。

采用同步停止，需等待当前正在进行的任务操作完成

---
## Signals:

|Return|Statement|Description|
|---|---|---|
|void|loginSucceed()|表示登录成功的信号|
|void|loginFail(const QString& err)|表示登录失败的信号|
|void|forbidInput()|表示请求禁止用户输入的信号|
|void|allowInput()|表示请求允许用户输入的信号|
|void|serverOutput(const QString& str)|表示请求输出服务器文本的信号|
|void|error(const QString& err)|表示请求错误处理的信号|
|void|end()|表示请求通信结束处理的信号|

---
### void loginSucceed()

表示登录成功的信号

---
### void loginFail(const QString& err)

表示登录失败的信号，并传递失败原因

#### parameter

* const QString& err - 错误原因

---
### void forbidInput()

表示请求禁止用户输入的信号

---
### void allowInput()

表示请求允许用户输入的信号

---
### void serverOutput(const QString& str)

表示请求输出服务器文本的信号，并传递接收到的服务器文本

#### parameter

* const QString& str - 服务器文本

---
### void error(const QString& err)

表示请求错误处理的信号，并传递错误原因

#### parameter

* const QString& err - 错误原因

---
### void end()

表示请求通信结束处理的信号

---
## Public Slots

|Return|Statement|Description|
|---|---|---|
|void|chat()|与服务器开始“聊天”|
|void|login(const QString& id)|处理用户登录|

---
### void chat()

与服务器开始“聊天”，处理服务器发送的任务

---
### void login(const QString& id)

处理用户登录，向服务器发送用户登录请求

#### parameter

* const QString& id - 用户id

---
## Private Types

|Type|Name|
|---|---|
|enum|Operation|

---
### enum Operation

服务器发送的任务操作类型

|Name|Description|
|---|---|
|EXIT|通信结束|
|SPEAK|输出服务器文本|
|LISTEN|获取用户输入|

---
## Private Functions

|Return|Statement|Description|
|---|---|---|
|void|initSocket()|初始化SOCKET|
|void|sendInputBuf()|发送用户缓冲区内容到服务器|

---
### void initSocket()

初始化与服务器通信SOCKET

---
### void sendInputBuf()

发送用户缓冲区内容到服务器