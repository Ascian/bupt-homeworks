#include"ManagerModule.h"

bool ManagerModule::execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx)
{
    ostringstream outBuf;
    istringstream recvInf(recvBuf);
    char event;
    recvInf.get(event);
    switch (event) {
    case RETURN: {
        return true;
        break;
    }
    case CHECK: {
        outBuf << pLogistics->manager;
        send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        break;
    }
    case PASSWORD: {
        string password;
        recvInf >> password;
        pLogistics->manager.setPassword(password);
        break;
    }
    case NAME: {
        string name;
        recvInf >> name;
        pLogistics->manager.setName(name);
        break;
    }
    case ADDCOURIER: {
        string username;
        string password;
        string name;
        string phone;
        recvInf >> username >> password >> name >> phone;
        try {
            mutx.lock();
            pLogistics->createCourierAccount(username, password, name, stoull(phone));
            mutx.unlock();
            cout << "A new courier \"" << username << "\" added" << endl;
            char outBuf = SUCCESS;
            send(pClient->cliSock, &outBuf, 1, 0);
        }
        catch (const char msg) {
            mutx.unlock();
            send(pClient->cliSock, &msg, 1, 0);
        }
        break;
    }
    case DELETECOURIER: {
        string username;
        recvInf >> username;
        try {
            mutx.lock();
            pLogistics->deleteCourierAccount(username);
            mutx.unlock();
            cout << "Courier \"" << username << "\" deleted" << endl;
            char outBuf = SUCCESS;
            send(pClient->cliSock, &outBuf, 1, 0);
        }
        catch (const char msg) {
            mutx.unlock();
            send(pClient->cliSock, &msg, 1, 0);
        }
        break;
    }
    case ASSIGN: {
        string username;
        recvInf >> username;
        display(pLogistics, pClient, 0, DSPLYNAEXP);

        unsigned int courierNum;
        recv(pClient->cliSock, (char*)&courierNum, 4, 0);

        Express* pExpress;
        try {
            pExpress = pLogistics->findExpress(courierNum);
        }
        catch (const char msg) {
            send(pClient->cliSock, &msg, 1, 0);
            break;
        }

        try {
            mutx.lock();
            pLogistics->assignExpress(pExpress, username);
            mutx.unlock();
            cout << "Express \"" << setfill('0') << setw(10) << courierNum << 
                "\" assigned to courier \"" << username << "\"" << endl;
            char outBuf = SUCCESS;
            send(pClient->cliSock, &outBuf, 1, 0);
        }
        catch (const char msg) {
            mutx.unlock();
            send(pClient->cliSock, &msg, 1, 0);
        }
        break;
    }
    case DSPLYUSER: {
        display(pLogistics, pClient, 0, DSPLYUSER);
        break;
    }
    case DSPLYCOURIER: {
        display(pLogistics, pClient, 0, DSPLYCOURIER);
        break;
    }
    case DSPLYEXP: {
        display(pLogistics, pClient, 0, DSPLYEXP);
        break;
    }
    case SRCHCREATETM: {
        search(pLogistics, pClient, 0, recvInf, SRCHCREATETM);
        break;
    }
    case FINDUSER: {
        string username;
        recvInf >> username;
        try {
            pClient->pUser = pLogistics->findUser(username);
            currentModule = 0;
            char outBuf = SUCCESS;
            send(pClient->cliSock, &outBuf, 1, 0);
        }
        catch (const char msg) {
            send(pClient->cliSock, &msg, 1, 0);
        }
        break;
    }
    case FINDCOURIER: {
        string username;
        recvInf >> username;
        try {
            pClient->pCourier = pLogistics->findCourier(username);
            currentModule = 1;
            char outBuf = SUCCESS;
            send(pClient->cliSock, &outBuf, 1, 0);
        }
        catch (const char msg) {
            send(pClient->cliSock, &msg, 1, 0);
        }
        break;
    }
    case FINDEXP: {
        findExpress(pLogistics, pClient, recvInf);
        break;
    }
    default:
        return false;
    }
    return false;
}
