#include "chat_processor.h"

#include <string>

#include <QDebug>

//服务器ip地址
extern const std::string kServerIp;
//服务器端口号
extern const int kServerPort;
//网络通信接收缓冲区大小
const int kBufSize = 500;

void ChatProcessor::initSocket()
{
    qInfo() << "Begin init socket, server IP: " << QString::fromStdString(kServerIp) << " server port: " << kServerPort;
    WSADATA wsaData;
    int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0)
    {
        qCritical() << "WSAStartup failed";
        emit error(tr("连接失败"));
    }
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        WSACleanup();
        qCritical() << "WSAStartup failed";
        emit error(tr("连接失败"));
    }

    cliSock = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN servAdd;
    servAdd.sin_family = AF_INET;
    servAdd.sin_addr.S_un.S_addr = inet_addr(kServerIp.c_str());
    servAdd.sin_port = htons(kServerPort);

    if (::connect(cliSock, (SOCKADDR*)&servAdd, sizeof(servAdd)))
    {
        qCritical() << "Socket connect failed: " << WSAGetLastError();
        emit error(tr("连接失败"));
        WSACleanup();
    }
    qInfo() << "Socket init succeeded";
}

ChatProcessor::ChatProcessor()
{
    initSocket();
    isClose = false;
}

ChatProcessor::~ChatProcessor()
{
    closesocket(cliSock);
    WSACleanup();
}

void ChatProcessor::login(const QString &id)
{
    std::string str = id.toStdString();
    // |id(1~kMaxIdLength)| -> server
    send(cliSock, str.c_str(), str.size(), 0);

    char IsLogin;
    // |true/false(1)| <- server
    int len = recv(cliSock, &IsLogin, 1, 0);
    if (len <= 0)
    {
        qCritical() << "recv failed:" << WSAGetLastError();
        emit error(tr("断开连接"));
        return;
    }
    //登陆失败，用户id不存在
    if (!IsLogin)
    {
        qWarning() << id << "id does not exist";
        emit loginFail(tr("ID不存在"));
        return;
    }

    qInfo() << "login succeed, id:" << id;
    emit loginSucceed();
}

void ChatProcessor::chat()
{
    qInfo() << "begin chat";
    emit forbidInput();  //禁止用户输入
    char buf[kBufSize];
    bool isReturn = false;
    //isClose为true时，强制停止与服务器通信
    while (!isReturn && !isClose)
    {
        // |op(1)|detail(0~(kBufSize-1))| <- server
        int len = recv(cliSock, buf, kBufSize, 0);
        if (len <= 0)
        {
            qCritical() << "recv failed:" << WSAGetLastError();
            emit error(tr("断开连接"));
            return;
        }
        switch (buf[0])
        {
        case EXIT:
            isReturn = 1;
            break;
        case SPEAK:
        {
            //buf = |SPEAK(1)|text(0~(kBufSize-1))|
            QString str = QString::fromLocal8Bit(buf + 1, len - 1);
            qDebug() << "server output:" << str;
            emit serverOutput(str);
            char ack = 1;
            // |ACK(1)| -> server
            send(cliSock, &ack, 1, 0);
            break;
        }
        case LISTEN:
        {
            // buf = |LISTEN(1)|beginTimer(4)|endTimer(4)|
            int beginTimer = *((int*)(buf + 1));
            int endTimer = *((int*)(buf + 5));
            qDebug() << "input timer:" << beginTimer << "," << endTimer;
            inputBuf = "";
            Sleep(beginTimer * 1000);
            emit allowInput();  //时限内允许用户输入
            Sleep((endTimer - beginTimer) * 1000);
            emit forbidInput();  //时限结束，再次禁止用户输入
            sendInputBuf();
            break;
        }
        }
    }
    qInfo() << "finish chat";
    emit end();
}

void ChatProcessor::sendInputBuf()
{
    std::string str(inputBuf.toLocal8Bit());
    qDebug() << "user input:" << inputBuf;
    str.insert(str.begin(), str.size() > 0);  //插入用户输入是否为空信息
    // |isEmpty(1)|text(0~)| -> server
    send(cliSock, str.c_str(), str.size(), 0);
}
