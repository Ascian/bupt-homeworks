#include <WinSock2.h> 

#include <iostream>

using namespace std;

const string kServerIp = "127.0.0.1";
const int kServerPort = 5500;
const int kMaxBufSize = 500;

enum Operation
{
    //通信结束
    EXIT,
    //输出应答文本
    SPEAK,
    //获取用户输入
    LISTEN,
};

//初始化SOCKET
void initSocket(SOCKET& servSock, SOCKET& cliSock);

//服务器模拟测试桩
main()
{
    SOCKET servSock;
    SOCKET cliSock;
    initSocket(servSock, cliSock);
    //获取用户id
    char buf[kMaxBufSize];
	int len = recv(cliSock, buf, kMaxBufSize, 0);
    if (len <= 0)
    {
        cout << "Socket Error: " << WSAGetLastError();
        return 0;
    }
    string id(buf, len);
    cout << "id: " << id << endl;
    char t = true;
	send(cliSock, &t, 1, 0);

    //输出输入文本
    while(true)
    {
        string op;
        cin >> op;
        //执行speak指令
        if(op == "speak")
        {
            string text;
            cin >> text;
            // |SPEAK(1)|text(0~(kMaxBufSize-1))| -> client
            buf[0] = SPEAK;
            int len = kMaxBufSize;
            if (text.size() < kMaxBufSize - 1)
                len = text.size() + 1;
            memcpy(buf + 1, text.c_str(), len - 1);
            send(cliSock, buf, len, 0);
            // |ACK(1)| <- client
            char ack;
	        len = recv(cliSock, &ack, 1, 0);
            if (len <= 0)
            {
                cout << "Socket Error: " << WSAGetLastError();
                return 0;
            }
        }
        //执行listen指令
        else if(op == "listen")
        {
            int beginTimer, endTimer;
            cin >> beginTimer >> endTimer;
            char outputBuf[9];
            // |LISTEN(1)|beginTimer(4)|endTimer(4)| -> client
            outputBuf[0] = LISTEN;
            memcpy(outputBuf + 1, &beginTimer, 4);
            memcpy(outputBuf + 5, &endTimer, 4);
            send(cliSock, outputBuf, 9, 0);
            char* inputBuf = new char[kMaxBufSize]; 
            // |isEmpty(1)|text(0~(kMaxBufSize-1))| <- client
	        int len = recv(cliSock, inputBuf, kMaxBufSize, 0);
            if (len <= 0)
            {
                cout << "Socket Error: " << WSAGetLastError();
                return 0;
            }
            string str;
            if (inputBuf[0])
		        str.assign(inputBuf + 1, len - 1);
	        else
		        str.clear();
            cout << str << endl;
        }
        //执行exit指令
        else if(op == "exit")
        {
            // |Exit(1)| -> client
            char exit = EXIT;
		    send(cliSock, &exit, 1, 0);
            break;
        }
    }

    closesocket(servSock);
    closesocket(cliSock);
    WSACleanup();
}

void initSocket(SOCKET& servSock, SOCKET& cliSock)
{
    WSADATA wsaData;
    int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0) {
        cout << "WSAStartup failed";
        exit(0);
    }
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        WSACleanup();
        cout << "WSAStartup failed";
        exit(0);
    }

    servSock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN servAdd;
    servAdd.sin_family = AF_INET;
    servAdd.sin_addr.S_un.S_addr = inet_addr(kServerIp.c_str());
    servAdd.sin_port = htons(kServerPort);
    if (::bind(servSock, (SOCKADDR*)&servAdd, sizeof(servAdd)) == SOCKET_ERROR) {
        cout << "Socket bind failed: " << WSAGetLastError();
        WSACleanup();
        exit(0);
    }
    if (listen(servSock, SOMAXCONN) == SOCKET_ERROR) {
        cout << "Socket listen failed: " << WSAGetLastError();
        WSACleanup();
        exit(0);
    }

    SOCKADDR_IN cliAdd;
    int len = sizeof(SOCKADDR);
    cliSock = accept(servSock, (SOCKADDR*)&cliAdd, &len);
    if (cliSock == SOCKET_ERROR) {  
        cout << "Accept client failed: " << WSAGetLastError();
        WSACleanup();
        exit(0);
    }
}