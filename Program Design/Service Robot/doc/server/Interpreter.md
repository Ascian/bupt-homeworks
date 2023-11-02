# Interpreter Class

脚本语法树的解释器，用于解释执行脚本

example:

    //首先需要指定脚本语法树
    SyntaxTree* Interpreter::kSyntaxTree = syntaxTree;
    //然后初始化解释器，提供输入输出,获取变量名对应的具体值和结束操作的回调函数
    interpreter.init(inputFunc, outputFunc, exitFunc, getVarsFunc);
    //随后可以开始解释执行脚本语法树
    interpreter.process();

- [Properties](#properties)
- [Public Functions](#public-functions)
  - [Interpreter()](#interpreter)
  - [void init(const function\<void(string\*, const int\&, const int\&)\>\& inputFunc, const function\<void(const string\&)\>\& outputFunc, const function\<void()\>\& exitFunc, const function\<void(map\<string, string\>\*, const vector\&)\>\& getVarsFunc)](#void-initconst-functionvoidstring-const-int-const-int-inputfunc-const-functionvoidconst-string-outputfunc-const-functionvoid-exitfunc-const-functionvoidmapstring-string-const-vector-getvarsfunc)
  - [void process()](#void-process)
- [Private Functions](#private-functions)
  - [void output(const Step::Expression\& exp)](#void-outputconst-stepexpression-exp)
  - [void input(string\* str, const int\& beginTimer, const int\& endTimer)](#void-inputstring-str-const-int-begintimer-const-int-endtimer)
  - [void exit()](#void-exit)



---
## Properties

|Type|Name|Description|
|---|---|---|
|static SyntaxTree*|kSyntaxTree|语法分析树|
|map\<string, string>|varTable|变量表，包含键值对{变量名, 变量值}|
|function\<void(string*, const int&, const int&)>|inputFunc|输入回调函数|
|function\<void(const string&)>|outputFunc|输出回调函数|
|function\<void()>|exitFunc|结束操作回调函数|

---
## Public Functions

|Return|Statement|Description|
|---|---|---|
||Interpreter()||
|void|init(const function<void(string*, const int&, const int&)>& inputFunc, const function<void(const string&)>& outputFunc, const function<void()>& exitFunc, const function<void(map<string, string>*, const vector<string>&)>& getVarsFunc)|初始化解释器|
|void|process()|解释执行脚本语法树|

---
### Interpreter()

构建一个脚本解释器，需要设置语法分析树并初始化后才可开始工作

---
### void init(const function<void(string*, const int&, const int&)>& inputFunc, const function<void(const string&)>& outputFunc, const function<void()>& exitFunc, const function<void(map<string, string>*, const vector<string>&)>& getVarsFunc)

初始化解释器，初始化后解释器才可以正常工作

#### parameter

* const function<void(string*, const int&, const int&)>& inputFunc - 输入回调函数
* const function<void(const string&)>& outputFunc - 输出回调函数
* const function<void()>& exitFunc - 结束操作回调函数
* const function<void(map<string, string>*, const vector<string>&)>& getVarsFunc - 获取变量名具体值的回调函数

---
### void process()

解释执行脚本语法树

执行步骤：当前Step置为entryStep循环，针对当前Step做
* 输出应答文本
* 如果本步骤是终结步骤，则结束循环，断开通话
* 获取用户输入
* 获得下一个Step

---

## Private Functions

|Return|Statement|Description|
|---|---|---|
|void|output(const Step::Expression& exp)|输出应答文本|
|void|input(string* str, const int& beginTimer, const int& endTimer)|获取用户输入|
|void|exit()|执行结束|

---
### void output(const Step::Expression& exp)

输出应答文本

#### parameter

* const Step::Expression& exp - 应答文本表达式

---
### void input(string* str, const int& beginTimer, const int& endTimer) 

获取用户输入

#### parameter

* string* str - 用户输入
* const int& beginTimer - 开始输入时间
* const int& endTimer - 结束输入时间

---
### void exit()

脚本解释执行结束，执行结束操作