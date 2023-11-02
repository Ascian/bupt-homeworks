#pragma once

#include "ConsoleModule.h"
#include"SenderModule.h"
#include"ReceiverModule.h"

class UserModule :
    public ConsoleModule
{
public:
    UserModule()
        :ConsoleModule("user") {
        commands = { "return", "check", "password", "name", "phone", "address", "recharge",
             "send", "signfor", "sendinfrm", "receiveinfrm", "findexp", "help" };
        subModules = { new SenderModule(), new ReceiverModule() };
        ban = { {}, {} };
    }

    virtual bool execute(SOCKET& cliSock, const string& command, const set<int>& ban);
};

inline const vector<string> Express_subKind() {
    return { "normal", "fragile", "book" };
}