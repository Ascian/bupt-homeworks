#pragma once

#include "ConsoleModule.h"
#include"SenderModule.h"
#include"ReceiverModule.h"


class UserModule :
    public ConsoleModule
{
public:
    UserModule() {
        subModules = { new SenderModule(), new ReceiverModule() };
    }

    virtual bool execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx);
};

