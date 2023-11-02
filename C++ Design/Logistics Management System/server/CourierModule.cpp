#include "CourierModule.h"

bool CourierModule::execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx)
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
        outBuf << *(pClient->pCourier);
        send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        break;
    }
    case PASSWORD: {
        string password;
        recvInf >> password;
        pClient->pCourier->setPassword(password);
        break;
    }
    case NAME: {
        string name;
        recvInf >> name;
        pClient->pCourier->setName(name);
        break;
    }
    case PHONE: {
        string phone;
        recvInf >> phone;
        pClient->pCourier->setPhone(stoll(phone));
        break;
    }
    case PICKUP: {
        display(pLogistics, pClient, 1, DSPLYNPEXP);

        while (true) {
            unsigned int courierNum;
            int len = recv(pClient->cliSock, (char*)&courierNum, 4, 0);
            if (len < 4)
                break;
            try {
                Express* pExpress = pLogistics->findExpress(courierNum);
                if (pExpress->getCourier() == pClient->pCourier->getUsername()) {
                    if (pExpress->getSendTime() != NOT_SENDED) {
                        char msg = EXPRESS_ALREADY_PICKUP;
                        send(pClient->cliSock, &msg, 1, 0);
                    }
                    else {
                        mutx.lock();
                        pLogistics->pickUpExpress(pExpress);
                        mutx.unlock();
                        cout << "Express \"" << setfill('0') << setw(10) << courierNum << "\" picked up" << endl;
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
    case DSPLYNPEXP: {
        display(pLogistics, pClient, 1, DSPLYNPEXP);
        break;
    }
    case DSPLYNREXP: {
        display(pLogistics, pClient, 1, DSPLYNREXP);
        break;
    }
    case DSPLYREXP: {
        display(pLogistics, pClient, 1, DSPLYREXP);
        break;
    }
    case SRCHSENDER: {
        search(pLogistics, pClient, 1, recvInf, SRCHSENDER);
        break;
    }
    case SRCHRECEIVER: {
        search(pLogistics, pClient, 1, recvInf, SRCHRECEIVER);
        break;
    }
    case SRCHPICKTM: {
        search(pLogistics, pClient, 1, recvInf, SRCHPICKTM);
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
