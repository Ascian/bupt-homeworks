# Logger Class

日志类，输出调试信息到标准输出和日志文件

example:

    //初始化日志对象
    logger.setup(logDicPath, logGenInterval, logMaxNum);
    //输出错误信息
    logger.error("error");
    //输出注意信息
    logger.note("note");
    //关闭日志对象
    logger.close();

- [Properties](#properties)
- [Public Functions](#public-functions)
  - [Logger()](#logger)
  - [void setup(const string\& logDicPath, const int\& logGenInterval, const int\& logMaxNum)](#void-setupconst-string-logdicpath-const-int-loggeninterval-const-int-logmaxnum)
  - [void close()](#void-close)
  - [void error(const string\& message)](#void-errorconst-string-message)
  - [void note(const string\& message)](#void-noteconst-string-message)
- [Private Functions](#private-functions)
  - [void output(const string\& type, const string\& message)](#void-outputconst-string-type-const-string-message)
  - [void checkLogNum()](#void-checklognum)
- [Friend Private Functions](#friend-private-functions)
  - [friend void generateLog(Logger\* logger)](#friend-void-generateloglogger-logger)



---
## Properties

|Type|Name|Description|
|---|---|---|
|string|logDicPath|日志文件目录地址|
|ofstream|logFile|日志文件流|
|mutex|mutx|日志输出锁|
|int|logGenInterval|新建日志文件间隔, 单位为天|
|int|logMaxNum|目录下最大日志文件数量|

---
## Public Functions

|Return|Statement|Description|
|---|---|---|
||Logger()||
|void|setup(const string& logDicPath, const int& logGenInterval, const int& logMaxNum)|初始化日志对象|
|void|close()|关闭日志对象|
|void|error(const string& message)|输出错误信息|
|void|note(const string& message)|输出注意信息|

---
### Logger()

构建一个日志对象，需初始化后才可开始使用

---
### void setup(const string& logDicPath, const int& logGenInterval, const int& logMaxNum)

初始化日志对象，建立日志文件输出流

#### parameter

* const string& logDicPath - 日志文件目录地址
* const int& logGenInterval - 新建日志文件间隔, 单位为天
* const int& logMaxNum - 目录下最大日志文件数量

---
### void close()

关闭日志文件输出流

---
### void error(const string& message)

输出错误信息

#### parameter:
* const string& message - 信息

---
### void note(const string& message)

输出注意信息

#### parameter:
* const string& message - 信息

---
## Private Functions

|Return|Statement|Description|
|---|---|---|
|void|output(const string& type, const string& message)|输出信息到标准输出和日志文件流|
|void|checkLogNum()|检查日志文件目录下文件数量是否超过最大值|

---
### void output(const string& type, const string& message)

输出信息到标准输出和日志文件输出流

#### parameter

* const string& type - 信息类型
* const string& message - 信息内容

---
### void checkLogNum()

检查日志文件目录下文件数量是否超过最大值，若是，删除最早日志文件

--- 
## Friend Private Functions

|Return|Statement|Description|
|---|---|---|
|friend void|generateLog(Logger* logger)|新建日志文件|

---
### friend void generateLog(Logger* logger)

新建日志文件

用于多线程处理每经过新建日志文件间隔，创建一个新的日志文件。

由于建立线程的函数参数不能为类成员，但该函数和该类关系紧密，需要使用到类数据成员。因此声明其为友元函数，并定义在类定义外。