#include "ReceiverModule.h"

bool ReceiverModule::execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx)
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
    case DSPLYNPEXP: {
        display(pLogistics, pClient, 2, DSPLYNPEXP);
        break;
    }

    case DSPLYNREXP: {
        display(pLogistics, pClient, 2, DSPLYNREXP);
        break;
    }
    case DSPLYREXP: {
        display(pLogistics, pClient, 2, DSPLYREXP);
        break;
    }
    case SRCHSENDER: {
        search(pLogistics, pClient, 2, recvInf, SRCHSENDER);
        break;
    }
    case SRCHRECEIVETM: {
        search(pLogistics, pClient, 2, recvInf, SRCHRECEIVETM);
        break;
    }
    default:
        return false;
    }
    return false;
}
