#include "UserModule.h"

bool UserModule::execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx)
{
    ostringstream outBuf;
    istringstream recvInf(recvBuf);
    char event;
    recvInf.get(event);
    switch (event) {
    case RETURN:
        return true;
        break;
    case CHECK: {
        outBuf << *(pClient->pUser);
        send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        break;
    }
    case PASSWORD: {
        string password;
        recvInf >> password;
        pClient->pUser->setPassword(password);
        break;
    }
    case NAME: {
        string name;
        recvInf >> name;
        pClient->pUser->setName(name);
        break;
    }
    case PHONE: {
        string phone;
        recvInf >> phone;
        pClient->pUser->setPhone(stoll(phone));
        break;
    }
    case ADDRESS: {
        string address;
        recvInf >> address;
        pClient->pUser->setAddress(address);
        break;
    }
    case RECHARGE: {
        string money;
        recvInf >> money;
        pClient->pUser->addBalance(stoul(money));
        break;
    }
    case SEND: {
        string receiver;
        string condition;
        string description;
        string kind;
        string amount;

        recvInf >> receiver >> condition >> description >> kind >> amount;
        try {
            mutx.lock();
            unsigned int courierNum = pLogistics->createLogistics(pClient->pUser->getUsername(), receiver, condition, description, stoul(amount), kind);
            mutx.unlock();
            cout << "A new express \"" << setfill('0') << setw(10) << courierNum << "\" added" << endl;
            char msg = SUCCESS;
            send(pClient->cliSock, &msg, 1, 0);
        }
        catch (const char msg) {
            mutx.unlock();
            send(pClient->cliSock, &msg, 1, 0);
        }
        break;
    }
    case SIGNFOR: {
        display(pLogistics, pClient, 2, DSPLYNREXP);

        while (true) {
            unsigned int courierNum;
            int len = recv(pClient->cliSock, (char*)&courierNum, 4, 0);
            if (len < 4)
                break;
            try {
                Express* pExpress = pLogistics->findExpress(courierNum);
                if (pExpress->getReceiver() == pClient->pUser->getUsername()) {
                    if (pExpress->getSendTime() == NOT_ASSIGNED || pExpress->getSendTime() == NOT_SENDED) {
                        char msg = EXPRESS_FORBIT_SIGNFOR;
                        send(pClient->cliSock, &msg, 1, 0);
                    }
                    else if (pExpress->getReceiveTime() != NOT_RECEIVED) {
                        char msg = EXPRESS_ALREADY_SIGNFOR;
                        send(pClient->cliSock, &msg, 1, 0);
                    }
                    else {
                        mutx.lock();
                        pLogistics->signForExpress(pExpress);
                        mutx.unlock();
                        cout << "Express \"" << setfill('0') << setw(10) << courierNum << "\" signed for" << endl;
                        char msg = SUCCESS;
                        send(pClient->cliSock, &msg, 1, 0);
                    }
                }
                else {
                    char msg = ITEM_NOT_BELONG_TO_YOU;
                    send(pClient->cliSock, &msg, 1, 0);
                }
            }
            catch (const char msg) {
                send(pClient->cliSock, &msg, 1, 0);
            }
        }
        break;
    }
    case SENDINFRM: {
        currentModule = 0;
        break;
    }
    case RECEIVEINFRM: {
        currentModule = 1;
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
