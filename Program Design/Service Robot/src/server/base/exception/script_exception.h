//定义脚本相关异常
//包括分析错误，文件打开错误，声明变量不存在

#ifndef SCRIPT_EXCEPTION_H_
#define SCRIPT_EXCEPTION_H_

#include <exception>
#include <string>

using namespace std;

//脚本相关异常
class ScriptException : public exception
{
public:
    //parameter:
    //    const string& message - 异常描述
    ScriptException(const string& message)
    {
        this->message = message;
    }

    inline const char* what() const throw ()
    {
        return message.c_str();
    }

private:
    //异常信息
    string message;
};

//脚本分析错误异常
class ScriptParseException : public ScriptException
{
public:
    //parameter:
    //    const int& line - 脚本第几行
    //    const int& pos - 第几个字符
    //    const string& message - 描述
    ScriptParseException(const int& line, const int& pos, const string& message)
        :ScriptException("(line " + to_string(line) + ", pos " + to_string(pos) + ") " + message) {}

    //parameter:
    //    const int& line - 脚本第几行
    //    const string& message - 描述
    ScriptParseException(const int& line, const string& message)
        :ScriptException("(line " + to_string(line) + ") " + message) {}

    //parameter:
    //    const string& message - 描述
    ScriptParseException(const string& message)
        :ScriptException(message) {}
};

//脚本文件打开异常
class ScriptFileException :
    public ScriptException
{
public:
    //parameter:
    //    const string& message - 描述
    //    const string& filePath -文件地址
    ScriptFileException(const string& message, const string& filePath)
        :ScriptException(message + " \"" + filePath + "\"") {}
};

//脚本声明变量不存在异常
class ScriptVariableNotExistException :
    public ScriptException
{
public:
    //parameter:
    //    const string& varName - 变量名
    ScriptVariableNotExistException(const string& varName)
        :ScriptException("variable \"" + varName + "\" does not exists") {}
};

#endif  //SCRIPT_EXCEPTION_H_
