#pragma once
#include "ConsoleModule.h"

class ReceiverModule :
    public ConsoleModule
{
public:
    ReceiverModule()
        :ConsoleModule("receiver") {
        commands = { "return", "dsplynpexp", "dsplynrexp", "dsplyrexp",
            "srchsender", "srchreceivetm", "help" };
    }

    virtual bool execute(SOCKET& cliSock, const string& command, const set<int>& ban);
};

