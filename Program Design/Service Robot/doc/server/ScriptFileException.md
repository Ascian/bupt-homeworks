# ScriptFileException Class : ScriptException

Inherits: [ScriptException](ScriptException.md)

脚本文件打开异常

- [Public Functions](#public-functions)
  - [ScriptFileException(const string\& message, const string\& filePath)](#scriptfileexceptionconst-string-message-const-string-filepath)
- [Inherited Public Functions](#inherited-public-functions)

---
## Public Functions

|Return|Statement|Description|
|---|---|---|
||ScriptFileException(const string& message, const string& filePath)||

---
### ScriptFileException(const string& message, const string& filePath)

#### parameter

* const string& message - 描述
* const string& filePath -文件地址

---
## Inherited Public Functions

|Return|Statement|Description|
|---|---|---|
|const char*|what() const throw ()|返回异常信息|