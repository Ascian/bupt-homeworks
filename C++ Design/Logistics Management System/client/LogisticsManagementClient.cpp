#include <iostream>
#include<string>
#include<Winsock2.h>

#pragma comment(lib,"ws2_32.lib") 

#include"MainModule.h"

using namespace std;

const string SERVER_ADD = "127.0.0.1";
const int SERVER_PORT = 5500;

MainModule mainModule;

int main()
{
    WSADATA wsaData;
    int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0) {
        cout << "WSAStartup failed" << endl;
        exit(-1);
    }
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        WSACleanup();
        cout << "WSAStartup failed" << endl;
        exit(-1);
    }

    SOCKET cliSock = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN servAdd;
    servAdd.sin_family = AF_INET;
    servAdd.sin_addr.S_un.S_addr = inet_addr(SERVER_ADD.c_str());
    servAdd.sin_port = htons(SERVER_PORT);

    if (connect(cliSock, (SOCKADDR*)&servAdd, sizeof(servAdd))) {
        cout << "Connect failed" << endl;
        WSACleanup();
        exit(-1);
    }

    bool ret = false;
    while (!ret) {
        time_t now = time(0);
        cout << "[" << put_time(localtime(&now), "%Y-%b-%d %H:%M:%S") << "] ";
        vector<string> commands = mainModule.getModuleCommand({});
        for (int i = 0; i < commands.size() - 1; i++) {
            cout << commands.at(i) << "/";
        }
        cout << commands.at(commands.size() - 1) << endl;

        cout << mainModule.getModuleName() << "> ";
        string command;
        cin >> command;
        ret = mainModule.parse(cliSock, command, {});
        cout << endl;
    }

    return 0;
}
