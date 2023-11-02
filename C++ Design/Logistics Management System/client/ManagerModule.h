#pragma once
#include "ConsoleModule.h"
#include "CourierModule.h"
#include "UserModule.h"


class ManagerModule :
    public ConsoleModule
{
public:
    ManagerModule()
        :ConsoleModule("manager") {
        commands = { "return", "check", "password", "name", "addcourier", "deletecourier",
            "assign", "dsplyuser", "dsplycourier", "dsplyexp", "srchcreatetm",
            "finduser", "findcourier", "findexpress", "help" };
        subModules = { new UserModule, new CourierModule() };
        ban = { { 2, 3, 4, 5, 6, 7, 8 }, { 2, 3, 4, 5 } };
    }

    virtual bool execute(SOCKET& cliSock, const string& command, const set<int>& ban);
};

