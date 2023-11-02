#include "SenderModule.h"

bool SenderModule::execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx)
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
    case DSPLYNSEXP: {
        display(pLogistics, pClient, 2, DSPLYNSEXP);
        break;
    }
    case DSPLYSEXP: {
        display(pLogistics, pClient, 2, DSPLYSEXP);
        break;
    }
    case SRCHRECEIVER: {
        search(pLogistics, pClient, 2, recvInf, SRCHRECEIVER);
        break;
    }
    case SRCHSENDTM: {
        search(pLogistics, pClient, 2, recvInf, SRCHSENDTM);
        break;
    }
    default:
        return false;
    }
    return false;
}
