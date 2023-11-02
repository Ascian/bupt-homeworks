//定义网络通信相关异常
//包括连接断开，连接错误

#ifndef SOCKET_EXCEPTION_H_
#define SOCKET_EXCEPTION_H_

#include <exception>
#include <string>

using namespace std;

//网络通信相关异常
class SocketException : public exception
{
public:
    //parameter:
    //    const string& message - 异常描述
    SocketException(const string& message)
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

//连接断开异常
class SocketDisconnectException :
    public SocketException
{
public:
    //parameter:
    //    const string& ip - 目的ip
    SocketDisconnectException(const string& ip)
        :SocketException("client " + ip + " disconnect") {}
};

//连接错误异常
class SocketErrorException :
    public SocketException
{
public:
    //parameter:
    //    const int& err - 错误编号
    SocketErrorException(const int& err)
        :SocketException("socket error: " + to_string(err)) {}
};

#endif  //SOCKET_EXCEPTION_H_