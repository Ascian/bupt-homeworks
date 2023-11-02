#ifndef CHAT_PROCESSOR_H_
#define CHAT_PROCESSOR_H_

#include <winsock2.h>

#include <QObject>
#include <QString>

//处理与服务器通信
//exmaple:
//    处理用户登录，向服务器发送用户登录请求
//    chatProcessor.login(userId);
//    与服务器开始“聊天”，处理服务器发送的任务
//    chatProcessor.chat();
class ChatProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ChatProcessor();
    ~ChatProcessor();

    //添加用户输入到缓冲区
    //parameter:
    //    const QString& input - 用户输入
    inline void addInputBuf(const QString& input) { this->inputBuf.append(input); }

    //强制停止工作
    void close() { isClose = true; }

signals:
    //表示登录成功的信号
    void loginSucceed();
    
    //表示登录失败的信号，并传递失败原因
    //parameter:
    //    const QString& err - 错误原因
    void loginFail(const QString& err);
    
    //表示请求禁止用户输入的信号
    void forbidInput();
    
    //表示请求允许用户输入的信号
    void allowInput();
    
    //表示请求输出服务器文本的信号，并传递接收到的服务器文本
    //parameter:
    //    const QString& str - 服务器文本
    void serverOutput(const QString& str);

    //表示请求错误处理的信号，并传递错误原因
    //parameter:
    //    const QString& err - 错误原因
    void error(const QString& err);
    
    //表示请求通信结束处理的信号
    void end();

public slots:
    //与服务器开始“聊天”，处理服务器发送的任务
    void chat();
    
    //处理用户登录，向服务器发送用户登录请求
    //parameter:
    //    const QString& id - 用户id
    void login(const QString& id);
    
private:
    //与服务器通信的SOCKET
    SOCKET cliSock;
    //用户输入缓冲区
    QString inputBuf;
    //是否强制停止工作
    bool isClose;

    //服务器发送的任务操作类型
    enum Operation
    {
        //通信结束
        EXIT,
        //输出服务器文本
        SPEAK,
        //获取用户输入
        LISTEN,
    };

    //初始化SOCKET
    void initSocket();
    //发送用户缓冲区内容到服务器
    void sendInputBuf();
};

#endif // CHAT_PROCESSOR_H_
