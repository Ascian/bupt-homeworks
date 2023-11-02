#include <iostream>
#include<string>
#include<Winsock2.h>
#include<thread>

#pragma comment(lib,"ws2_32.lib") 

#include"MainModule.h"

using namespace std;

const string SAVE_PATH = "./data";
const string SERVER_ADD = "127.0.0.1";
const int SERVER_PORT = 5500;
const int MAX_LINK_NUM = 10;

void initLogistics();
void serverWorking(Client* pClient);

mutex mutx;
Logistics* pLogistics;
Client clients[MAX_LINK_NUM];

int main()
{
    pLogistics = new Logistics();
    initLogistics();

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

    SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN servAdd;
    servAdd.sin_family = AF_INET;
    servAdd.sin_addr.S_un.S_addr = inet_addr(SERVER_ADD.c_str());
    servAdd.sin_port = htons(SERVER_PORT);

    if (bind(servSock, (SOCKADDR*)&servAdd, sizeof(servAdd)) == SOCKET_ERROR) {
        cout << "Bind failed" << endl;
        WSACleanup();
        exit(-1);
    }

    if (listen(servSock, SOMAXCONN) == SOCKET_ERROR) {
        cout << "Listen failed" << endl;
        WSACleanup();
        exit(-1);
    }

    SOCKADDR_IN cliAdd;
    int len = sizeof(SOCKADDR);

    while (1) {
        SOCKET cliSock = accept(servSock, (SOCKADDR*)&cliAdd, &len);
        if (cliSock == SOCKET_ERROR) {
            cout << "Accept failed: " << WSAGetLastError() << endl;
            break;
        }

        Client* pClient;
        for (int i = 0; i < MAX_LINK_NUM; i++) {
            if (clients[i].cliSock == INVALID_SOCKET) {
                clients[i].cliSock = cliSock;
                pClient = &clients[i];
                break;
            }
            if (i == MAX_LINK_NUM - 1) {
                Sleep(10);
                i = 0;
            }
        }

        thread th(&serverWorking, pClient);
        th.detach();
        cout << "Accept client IP: " << inet_ntoa(cliAdd.sin_addr) << endl;
    }

    closesocket(servSock);
    WSACleanup();

    delete(pLogistics);
    return 0;
}

void initLogistics() {
    try {
        pLogistics->load(SAVE_PATH);
        cout << "Load existed logistics database" << endl;
    }
    catch (const char* msg) {
        cout << msg << endl;
        cout << "Creat new logistics database" << endl;
        cout << "Please input the manager information" << endl;
        cout << "Username: ";
        string username;
        cin >> username;
        while (username.size() > STRING_MAX_SIZE) {
            cout << "Username length should be less than " << STRING_MAX_SIZE << endl;
            cout << "Username: ";
            cin >> username;
        }
        pLogistics->manager.setUsername(username);
        cout << "Password: ";
        string password;
        cin >> password;
        while (password.size() > STRING_MAX_SIZE) {
            cout << "Password length should be less than " << STRING_MAX_SIZE << endl;
            cout << "Password: ";
            cin >> password;
        }
        pLogistics->manager.setPassword(password);
        cout << "Name: ";
        string name;
        cin >> name;
        while (name.size() > STRING_MAX_SIZE) {
            cout << "Name length should be less than " << STRING_MAX_SIZE << endl;
            cout << "Name: ";
            cin >> name;
        }
        pLogistics->manager.setName(name);
    }
}

void serverWorking(Client* pClient)
{
    MainModule* pMainModule = new MainModule();
    char* recvBuf = new char[MAX_BUFFER_SIZE];
    bool ret = false;
    while (!ret) {
        int len = recv(pClient->cliSock, recvBuf, MAX_BUFFER_SIZE, 0);
        recvBuf[len] = 0;
        ret = pMainModule->parse(pLogistics, pClient, recvBuf, mutx);
    }
    delete(recvBuf);
    delete(pMainModule);
    closesocket(pClient->cliSock);
    pClient->cliSock = INVALID_SOCKET;
    pLogistics->save(SAVE_PATH);
}
