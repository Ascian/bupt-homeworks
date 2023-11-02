#include "MainModule.h"
bool MainModule::execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx)
{
    istringstream recvInf(recvBuf);
    char event;
    recvInf.get(event);
    switch (event) {
    case EXIT:
        return true;
        break;
    case USER: {
        string username;
        string password;
        recvInf >> username >> password;
        try {
            pClient->pUser = pLogistics->loginUserAccount(username, password);
            currentModule = 0;
            char outBuf = SUCCESS;
            send(pClient->cliSock, &outBuf, 1, 0);
        }
        catch (const char msg) {
            send(pClient->cliSock, &msg, 1, 0);
        }
        break;
    }
    case COURIER: {
        string username;
        string password;
        recvInf >> username >> password;
        try {
            pClient->pCourier = pLogistics->loginCourierAccount(username, password);
            currentModule = 1;
            char outBuf = SUCCESS;
            send(pClient->cliSock, &outBuf, 1, 0);
        }
        catch (const char msg) {
            send(pClient->cliSock, &msg, 1, 0);
        }
        break;
    }
    case MANAGER: {
        string username;
        string password;
        recvInf >> username >> password;
        try {
            pLogistics->loginManagerAccount(username, password);
            currentModule = 2;
            char outBuf = SUCCESS;
            send(pClient->cliSock, &outBuf, 1, 0);
        }
        catch (const char msg) {
            send(pClient->cliSock, &msg, 1, 0);
        }
        break;
    }
    case REGISTER: {
        string username;
        string password;
        string name;
        string phone;
        string address;
        recvInf >> username >> password >> name >> phone >> address;
        try {
            mutx.lock();
            pLogistics->createUserAccount(username, password, name, stoull(phone), address);
            mutx.unlock();
            cout << "A new user \"" << username << "\" added" << endl;
            char outBuf = SUCCESS;
            send(pClient->cliSock, &outBuf, 1, 0);
        }
        catch (const char msg) {
            mutx.unlock();
            send(pClient->cliSock, &msg, 1, 0);
        }

        break;
    }
    default:
        return false;
    }
    return false;
}
