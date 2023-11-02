#include "SenderModule.h"

bool SenderModule::execute(SOCKET& cliSock, const string& command, const set<int>& ban)
{
    ostringstream outBuf;
    if (command == commands.at(0) && !ban.contains(0)) {
        outBuf << RETURN;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        return true;
    }
    else if (command == commands.at(1) && !ban.contains(1)) {
        outBuf << DSPLYNSEXP;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        cout << "Not sended expresses:" << endl;
        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == CONTAINER_EMPTY) {
            cout << "No not-sended express" << endl;
            return false;
        }

        char recvBuf[MAX_BUFFER_SIZE];
        int len = recv(cliSock, recvBuf, MAX_BUFFER_SIZE, 0);
        recvBuf[len] = 0;
        cout << recvBuf << endl;

        recv(cliSock, &msg, 1, 0);
        while (msg) {
            cout << "Display more information(Y/N):";
            string more;
            cin >> more;
            while (more != "Y" && more != "y" && more != "N" && more != "n") {
                cout << "Input error" << endl;
                cout << "Display more information(Y/N):";
            }
            if (more == "Y" || more == "y") {
                msg = 1;
                send(cliSock, &msg, 1, 0);
                len = recv(cliSock, recvBuf, MAX_BUFFER_SIZE, 0);
                recvBuf[len] = 0;
                cout << recvBuf << endl;
            }
            else {
                msg = 0;
                send(cliSock, &msg, 1, 0);
            }
            recv(cliSock, &msg, 1, 0);
        }
    }
    else if (command == commands.at(2) && !ban.contains(2)) {
        outBuf << DSPLYSEXP;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        cout << "Sended expresses:" << endl;
        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == CONTAINER_EMPTY) {
            cout << "No sended express" << endl;
            return false;
        }

        char recvBuf[MAX_BUFFER_SIZE];
        int len = recv(cliSock, recvBuf, MAX_BUFFER_SIZE, 0);
        recvBuf[len] = 0;
        cout << recvBuf << endl;

        recv(cliSock, &msg, 1, 0);
        while (msg) {
            cout << "Display more information(Y/N):";
            string more;
            cin >> more;
            while (more != "Y" && more != "y" && more != "N" && more != "n") {
                cout << "Input error" << endl;
                cout << "Display more information(Y/N):";
            }
            if (more == "Y" || more == "y") {
                msg = 1;
                send(cliSock, &msg, 1, 0);
                len = recv(cliSock, recvBuf, MAX_BUFFER_SIZE, 0);
                recvBuf[len] = 0;
                cout << recvBuf << endl;
            }
            else {
                msg = 0;
                send(cliSock, &msg, 1, 0);
            }
            recv(cliSock, &msg, 1, 0);
        }
    }
    else if (command == commands.at(3) && !ban.contains(3)) {
        cout << "Please input the receiver's username: ";
        string reciever;
        cin >> reciever;
        outBuf << SRCHRECEIVER << reciever << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == ELEMENT_NOT_FOUND) {
            cout << "No express recieved by " << reciever << endl;
            return false;
        }

        char recvBuf[MAX_BUFFER_SIZE];
        int len = recv(cliSock, recvBuf, MAX_BUFFER_SIZE, 0);
        recvBuf[len] = 0;
        cout << recvBuf << endl;

        recv(cliSock, &msg, 1, 0);
        while (msg) {
            cout << "Display more information(Y/N):";
            string more;
            cin >> more;
            while (more != "Y" && more != "y" && more != "N" && more != "n") {
                cout << "Input error" << endl;
                cout << "Display more information(Y/N):";
            }
            if (more == "Y" || more == "y") {
                msg = 1;
                send(cliSock, &msg, 1, 0);
                len = recv(cliSock, recvBuf, MAX_BUFFER_SIZE, 0);
                recvBuf[len] = 0;
                cout << recvBuf << endl;
            }
            else {
                msg = 0;
                send(cliSock, &msg, 1, 0);
            }
            recv(cliSock, &msg, 1, 0);
        }
    }
    else if (command == commands.at(4) && !ban.contains(4)) {
        cout << "Please input the time information in format %Y-%m-%d %H:%M:%S" << endl;
        cout << "Lower Bound: ";
        char lowerBound[20];
        cin.getline(lowerBound, 20);
        cin.getline(lowerBound, 20);
        cout << "Upper Bound: ";
        char upperBound[20];
        cin.getline(upperBound, 20);

        outBuf << SRCHSENDTM << lowerBound << endl << upperBound << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == ELEMENT_NOT_FOUND) {
            cout << "No express sended between " << lowerBound << "and " << upperBound << endl;
            return false;
        }

        char recvBuf[MAX_BUFFER_SIZE];
        int len = recv(cliSock, recvBuf, MAX_BUFFER_SIZE, 0);
        recvBuf[len] = 0;
        cout << recvBuf << endl;

        recv(cliSock, &msg, 1, 0);
        while (msg) {
            cout << "Display more information(Y/N):";
            string more;
            cin >> more;
            while (more != "Y" && more != "y" && more != "N" && more != "n") {
                cout << "Input error" << endl;
                cout << "Display more information(Y/N):";
            }
            if (more == "Y" || more == "y") {
                msg = 1;
                send(cliSock, &msg, 1, 0);
                len = recv(cliSock, recvBuf, MAX_BUFFER_SIZE, 0);
                recvBuf[len] = 0;
                cout << recvBuf << endl;
            }
            else {
                msg = 0;
                send(cliSock, &msg, 1, 0);
            }
            recv(cliSock, &msg, 1, 0);
        }
    }
    else if (command == commands.at(5) && !ban.contains(5)) {
        cout << "Available commands:" << endl;
        if (!ban.contains(0))
            cout << "  " << commands.at(0) << ": return to the upper module" << endl;
        if (!ban.contains(1))
            cout << "  " << commands.at(1) << ": display all not sended expresses' message" << endl;
        if (!ban.contains(2))
            cout << "  " << commands.at(2) << ": display all sended expresses' message" << endl;
        if (!ban.contains(3))
            cout << "  " << commands.at(3) << ": search the related expresses by receiver" << endl;
        if (!ban.contains(4))
            cout << "  " << commands.at(4) << ": search the related expresses by sending time" << endl;
        if (!ban.contains(5))
            cout << "  " << commands.at(5) << ": print this help" << endl;
    }
    else
        cout << "Unknown command" << endl;
    return false;
}
