#pragma once

#include"ConsoleModule.h"
#include"UserModule.h"
#include"ManagerModule.h"
#include"CourierModule.h"

class MainModule :
    public ConsoleModule
{
public:
    MainModule()  {
        subModules = { new UserModule(), new CourierModule(), new ManagerModule()};
    }

    virtual bool execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx);
};

