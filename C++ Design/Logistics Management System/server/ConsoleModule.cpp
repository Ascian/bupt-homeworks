#include "ConsoleModule.h"

void ConsoleModule::display(Logistics* pLogistics, Client* pClient, const char kind, const char event)
{
    ostringstream outBuf;
    int i = 0;
    bool moreInf;
    try {
        if (kind == 0) {
            if (event == DSPLYUSER)
                moreInf = pLogistics->usersToString(outBuf, i * 10, i * 10 + 9);
            else if (event == DSPLYCOURIER)
                moreInf = pLogistics->couriersToString(outBuf, i * 10, i * 10 + 9);
            else if (event == DSPLYEXP)
                moreInf = pLogistics->expressesToString(outBuf, i * 10, i * 10 + 9);
            else if (event == DSPLYNAEXP)
                moreInf = pLogistics->notAExpToString(outBuf, i * 10, i * 10 + 9);
        }
        else if (kind == 1) {
            if (event == DSPLYNPEXP)
                moreInf = pClient->pCourier->notPExpToString(outBuf, i * 10, i * 10 + 9);
            else if (event == DSPLYNREXP)
                moreInf = pClient->pCourier->notRExpToString(outBuf, i * 10, i * 10 + 9);
            else if (event == DSPLYREXP)
                moreInf = pClient->pCourier->rExpToString(outBuf, i * 10, i * 10 + 9);
        }
        else if (kind == 2) {
            if (event == DSPLYNPEXP)
                moreInf = pClient->pUser->notPExpToString(outBuf, i * 10, i * 10 + 9);
            else if (event == DSPLYNREXP)
                moreInf = pClient->pUser->notRExpToString(outBuf, i * 10, i * 10 + 9);
            else if (event == DSPLYREXP)
                moreInf = pClient->pUser->rExpToString(outBuf, i * 10, i * 10 + 9);
            else if (event == DSPLYNSEXP)
                moreInf = pClient->pUser->notSExpToString(outBuf, i * 10, i * 10 + 9);
            else if (event == DSPLYSEXP)
                moreInf = pClient->pUser->sExpToString(outBuf, i * 10, i * 10 + 9);
        }

    }
    catch (const char msg) {
        send(pClient->cliSock, &msg, 1, 0);
        return;
    }

    char msg = SUCCESS;
    send(pClient->cliSock, &msg, 1, 0);

    i++;
    send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
    Sleep(10);
    msg = 1;
    while (moreInf) {
        send(pClient->cliSock, &msg, 1, 0);

        recv(pClient->cliSock, &msg, 1, 0);
        if (msg) {
            outBuf.str("");
            if (kind == 0) {
                if (event == DSPLYUSER)
                    moreInf = pLogistics->usersToString(outBuf, i * 10, i * 10 + 9);
                else if (event == DSPLYCOURIER)
                    moreInf = pLogistics->couriersToString(outBuf, i * 10, i * 10 + 9);
                else if (event == DSPLYEXP)
                    moreInf = pLogistics->expressesToString(outBuf, i * 10, i * 10 + 9);
                else if (event == DSPLYNAEXP)
                    moreInf = pLogistics->notAExpToString(outBuf, i * 10, i * 10 + 9);
            }
            else if (kind == 1) {
                if (event == DSPLYNPEXP)
                    moreInf = pClient->pCourier->notPExpToString(outBuf, i * 10, i * 10 + 9);
                else if (event == DSPLYNREXP)
                    moreInf = pClient->pCourier->notRExpToString(outBuf, i * 10, i * 10 + 9);
                else if (event == DSPLYREXP)
                    moreInf = pClient->pCourier->rExpToString(outBuf, i * 10, i * 10 + 9);
            }
            else if (kind == 2) {
                if (event == DSPLYNPEXP)
                    moreInf = pClient->pUser->notPExpToString(outBuf, i * 10, i * 10 + 9);
                else if (event == DSPLYNREXP)
                    moreInf = pClient->pUser->notRExpToString(outBuf, i * 10, i * 10 + 9);
                else if (event == DSPLYREXP)
                    moreInf = pClient->pUser->rExpToString(outBuf, i * 10, i * 10 + 9);
                else if (event == DSPLYNSEXP)
                    moreInf = pClient->pUser->notSExpToString(outBuf, i * 10, i * 10 + 9);
                else if (event == DSPLYSEXP)
                    moreInf = pClient->pUser->sExpToString(outBuf, i * 10, i * 10 + 9);
            }
            i++;
            send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
            Sleep(10);
        }
        else
            break;
    }
    msg = 0;
    send(pClient->cliSock, &msg, 1, 0);
    return;
}

void ConsoleModule::search(Logistics* pLogistics, Client* pClient, const char kind, istringstream& recvInf, const char event)
{
    ostringstream outBuf;
    struct tm lowerBound;
    struct tm upperBound;
    string id;
    if (event == SRCHCREATETM || event == SRCHPICKTM || event == SRCHRECEIVETM || event == SRCHSENDTM) {
        recvInf >> get_time(&lowerBound, "%Y-%m-%d %H:%M:%S");
        recvInf >> get_time(&upperBound, "%Y-%m-%d %H:%M:%S");
    }
    else {
        recvInf >> id;
    }

    vector<const Express*> expresses;
    try {
        if (kind == 0) {
            if (event == SRCHCREATETM)
                expresses = pLogistics->searchCreateTime(mktime(&lowerBound), mktime(&upperBound));
        }
        else if (kind == 1) {
            if (event == SRCHPICKTM)
                expresses = pClient->pCourier->searchPickTime(mktime(&lowerBound), mktime(&upperBound));
            if (event == SRCHSENDER)
                expresses = pClient->pCourier->searchSender(id);
            else if (event == SRCHRECEIVER)
                expresses = pClient->pCourier->searchReceiver(id);
        }
        else if (kind == 2) {
            if (event == SRCHSENDER)
                expresses = pClient->pUser->searchSender(id);
            else if (event == SRCHRECEIVER)
                expresses = pClient->pUser->searchReceiver(id);
            else if (event == SRCHRECEIVETM)
                expresses = pClient->pUser->searchReceiveTime(mktime(&lowerBound), mktime(&upperBound));
            else if (event == SRCHSENDTM)
                expresses = pClient->pUser->searchSendTime(mktime(&lowerBound), mktime(&upperBound));
        }

        char msg = SUCCESS;
        send(pClient->cliSock, &msg, 1, 0);
        int i = 0, j = 0;
        for (j = i * 10; j < i * 10 + 10 && j < expresses.size(); j++) {
            outBuf << "[" << j << "]" << endl;
            outBuf << *(Express*)expresses.at(j) << endl;
        }
        send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        Sleep(10);
        i++;
        msg = 1;
        while (j < expresses.size()) {
            send(pClient->cliSock, &msg, 1, 0);

            recv(pClient->cliSock, &msg, 1, 0);
            if (msg) {
                outBuf.str("");
                for (j = i * 10; j < i * 10 + 10 && j < expresses.size(); j++) {
                    outBuf << "[" << j << "]" << endl;
                    outBuf << *(Express*)expresses.at(j) << endl;
                }
                i++;
                send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
                Sleep(10);
            }
            else
                break;
        }
        msg = 0;
        send(pClient->cliSock, &msg, 1, 0);
    }
    catch (const char msg) {
        send(pClient->cliSock, &msg, 1, 0);
    }
    return;
}

void ConsoleModule::findExpress(Logistics* pLogistics, Client* pClient, istringstream& recvInf)
{
    ostringstream outBuf;
    string courierNum;
    recvInf >> courierNum;
    try {
        outBuf << *pLogistics->findExpress(stoul(courierNum)) << endl;
        char msg = SUCCESS;
        send(pClient->cliSock, &msg, 1, 0);
        send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
    }
    catch (const char msg) {
        send(pClient->cliSock, &msg, 1, 0);
    }
    return;
}
