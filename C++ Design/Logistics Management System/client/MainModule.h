#pragma once

#include "ConsoleModule.h"
#include"UserModule.h"
#include"ManagerModule.h"
#include"CourierModule.h"


class MainModule :
    public ConsoleModule
{
public:
    MainModule()
        :ConsoleModule("main") {
        commands = { "exit", "user", "courier", "manager", "register", "help" };
        subModules = { new UserModule(), new CourierModule(), new ManagerModule() };
        ban = { {},{},{} };
    }

    virtual bool execute(SOCKET& cliSock, const string& command, const set<int>& ban);
};

