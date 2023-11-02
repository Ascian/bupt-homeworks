# ChatWindow Class : QMainWindow

Inherits: [QMainWindow](https://doc.qt.io/qt-6/qmainwindow.html)

聊天窗口，用于显示服务器和用户文本，获取用户输入

example:

    //首先需要用户登录，若登录成功显示聊天窗口并开始与服务器交流，否则退出程序
    if(chatWindow.login())
    {
        chatWindow.show();
        chatWindow.start();
        return a.exec();
    }
    return 0;

- [Properties](#properties)
- [Public Functions](#public-functions)
  - [ChatWindow(QWidget \*parent = 0)](#chatwindowqwidget-parent--0)
  - [~ChatWindow()](#chatwindow)
  - [bool login()](#bool-login)
  - [void start()](#void-start)
- [Signals:](#signals)
  - [void beginChat()](#void-beginchat)
- [Reimplemented Protected Functions](#reimplemented-protected-functions)
  - [virtual void resizeEvent(QResizeEvent \*event)](#virtual-void-resizeeventqresizeevent-event)
- [Private Functions](#private-functions)
  - [void addText(ChatMessage\* message, const QString\& text)](#void-addtextchatmessage-message-const-qstring-text)
  - [void addTime()](#void-addtime)
- [Private Slots](#private-slots)
  - [void on\_enterButton\_clicked()](#void-on_enterbutton_clicked)
  - [void forbidInput()](#void-forbidinput)
  - [void allowInput()](#void-allowinput)
  - [void serverOutput(const QString\& text)](#void-serveroutputconst-qstring-text)
  - [void error(const QString\& err)](#void-errorconst-qstring-err)
  - [void end()](#void-end)


---
## Properties

|Type|Name|Description|
|---|---|---|
|Ui::ChatWindow*|ui||
|LoginDialog*|loginDialog|登录会话窗口|
|QThread|chatThread|处理与服务器通信的线程|
|ChatProcessor*|chatProcessor|处理与服务器通信的对象|

---
## Public Functions

|Return|Statement|Description|
|---|---|---|
||ChatWindow(QWidget *parent = 0)||
||~ChatWindow()||
|bool|login()|执行用户登录|
|void|start()|开始与服务器交流|

---
### ChatWindow(QWidget *parent = 0)

构建一个聊天窗口

---
### ~ChatWindow()

析构该聊天窗口，关闭处理与服务器通信的对象，等待处理与服务器通信的线程退出

---
### bool login()

显示用户登录会话窗口，执行用户登录

#### return

返回登录是否成功

---
### void start()

开始与服务器“聊天”，显示服务器和用户文本，获取用户输入

---
## Signals:

|Return|Statement|Description|
|---|---|---|
|void|beginChat()|表示与服务器“聊天”开始的信号|

---
### void beginChat()

表示与服务器“聊天”开始的信号

---
## Reimplemented Protected Functions

|Return|Statement|Description|
|---|---|---|
|virtual void|resizeEvent(QResizeEvent *event)|重新计算各窗口部件大小|

---
### virtual void resizeEvent(QResizeEvent *event)

Reimplements: QWidget::resizeEvent(QResizeEvent *event)

接受改变窗口部件大小的请求，重新计算各窗口部件大小

---
## Private Functions

|Return|Statement|Description|
|---|---|---|
|void|addText(ChatMessage* message, const QString& text)|添加显示文本|
|void|addTime()|添加显示时间|

---
### void addText(ChatMessage* message, const QString& text)

添加显示文本到显示列表

#### parameter

* ChatMessage* message - 文本显示窗口部件
* const QString& text - 文本内容

---
### void addTime()

若距离上一次显示文本内容超过一分钟，添加显示时间到显示列表

---
## Private Slots

|Return|Statement|Description|
|---|---|---|
|void|on_enterButton_clicked()|输入按键|
|void|forbidInput()|禁止用户输入|
|void|allowInput()|允许用户输入|
|void|serverOutput(const QString& text)|显示服务器文本内容|
|void|error(const QString& err)|程序执行出错|
|void|end()|结束与服务器通信|

---
### void on_enterButton_clicked()

输入按键

---
### void forbidInput()

隐藏输入文本框，禁止用户输入

---
### void allowInput()

显示输入文本框，允许用户输入

---
### void serverOutput(const QString& text)

显示服务器文本内容到显示列表

#### parameter

* const QString& text - 服务器文本内容

---
### void error(const QString& err)

程序执行出错，提醒用户后关闭窗口

#### parameter

* const QString& err - 错误描述

---
### void end()

结束与服务器通信，提醒用户后关闭窗口