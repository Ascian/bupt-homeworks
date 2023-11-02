# ScriptException Class : exception

Inherits: [exception](https://en.cppreference.com/w/cpp/error/exception)

脚本相关异常

- [Properties](#properties)
- [Public Functions](#public-functions)
  - [ScriptException(const string\& message)](#scriptexceptionconst-string-message)
  - [const char\* what() const throw ()](#const-char-what-const-throw-)


---
## Properties

|Type|Name|Description|
|---|---|---|
|string|message|异常信息|

---
## Public Functions

|Return|Statement|Description|
|---|---|---|
||ScriptException(const string& message)||
|const char*|what() const throw ()|返回异常信息|

---
### ScriptException(const string& message)

#### parameter

* const string& message - 异常描述

---
### const char* what() const throw ()

返回异常信息

