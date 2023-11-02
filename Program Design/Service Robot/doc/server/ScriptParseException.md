# ScriptParseException Class : ScriptException

Inherits: [ScriptException](ScriptException.md)

脚本分析错误异常

- [Public Functions](#public-functions)
  - [ScriptParseException(const int\& line, const int\& pos, const string\& message)](#scriptparseexceptionconst-int-line-const-int-pos-const-string-message)
  - [ScriptParseException(const int\& line, const string\& message)](#scriptparseexceptionconst-int-line-const-string-message)
  - [ScriptParseException(const string\& message)](#scriptparseexceptionconst-string-message)
- [Inherited Public Functions](#inherited-public-functions)


---
## Public Functions

|Return|Statement|Description|
|---|---|---|
||ScriptParseException(const int& line, const int& pos, const string& message)||
||ScriptParseException(const int& line, const string& message)||
||ScriptParseException(const string& message)||

---
### ScriptParseException(const int& line, const int& pos, const string& message)

#### parameter

* const int& line - 脚本第几行
* const int& pos - 第几个字符
* const string& message - 描述

---
### ScriptParseException(const int& line, const string& message)

#### parameter

* const int& line - 脚本第几行
* const string& message - 描述

---
### ScriptParseException(const string& message)

#### parameter

* const string& message - 描述

---
## Inherited Public Functions

|Return|Statement|Description|
|---|---|---|
|const char*|what() const throw ()|返回异常信息|
