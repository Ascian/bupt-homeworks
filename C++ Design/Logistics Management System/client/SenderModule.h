#pragma once
#include "ConsoleModule.h"

class SenderModule :
    public ConsoleModule
{
public:
    SenderModule()
        :ConsoleModule("sender") {
        commands = { "return", "dsplynsexp", "dsplysexp", "srchreceiver",
            "srchsendtm", "help" };
    }

    virtual bool execute(SOCKET& cliSock, const string& command, const set<int>& ban);
};

