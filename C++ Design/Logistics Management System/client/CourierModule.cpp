#include "CourierModule.h"

bool CourierModule::execute(SOCKET& cliSock, const string& command, const set<int>& ban)
{
    ostringstream outBuf;
    if (command == commands.at(0) && !ban.contains(0)) {
        outBuf << RETURN;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        return true;
    }
    else if (command == commands.at(1) && !ban.contains(1)) {
        outBuf << CHECK;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        char recvBuf[MAX_BUFFER_SIZE];
        int len = recv(cliSock, recvBuf, MAX_BUFFER_SIZE, 0);
        recvBuf[len] = 0;
        cout << recvBuf;
    }
    else if (command == commands.at(2) && !ban.contains(2)) {
        cout << "Please input your new password: ";
        string password;
        cin >> password;
        while (password.size() > STRING_MAX_SIZE) {
            cout << "Password length should be less than " << STRING_MAX_SIZE << endl;
            cout << "Please input your new password: ";
            cin >> password;
        }
        outBuf << PASSWORD << password << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
    }
    else if (command == commands.at(3) && !ban.contains(3)) {
        cout << "Please input your new name: ";
        string name;
        cin >> name;
        while (name.size() > STRING_MAX_SIZE) {
            cout << "Name length should be less than " << STRING_MAX_SIZE << endl;
            cout << "Please input your new name: ";
            cin >> name;
        }
        outBuf << NAME << name << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
    }
    else if (command == commands.at(4) && !ban.contains(4)) {
        cout << "Please input your new phone number: ";
        string phone;
        cin >> phone;
        regex r("^\\d+$");
        while (!regex_match(phone, r) || phone.length() > 11) {
            cout << "Format error, phone number consists of up to 11 digits" << endl;
            cout << "Please input your new phone number: ";
            cin >> phone;
        }
        outBuf << PHONE << phone << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
    }
    else if (command == commands.at(5) && !ban.contains(5)) {
        outBuf << PICKUP;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        cout << "Not picked-up expresses:" << endl;
        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == CONTAINER_EMPTY) {
            cout << "No not-picked-up express" << endl;
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

        cout << "Please input the courier numbers of the expresses you want to pick up" << endl;
        cout << "Note: input '#' as the end" << endl;
        unsigned int i = 1;
  
        while (true) {
            cout << i << ": ";
            string courierNum;
            cin >> courierNum;
            regex r("[0-9]{10}");
            while (!(regex_match(courierNum, r) || courierNum == "#")) {
                cout << "Format error, please input a 10-digits courier number or '#'" << endl;
                cout << i << ": ";
                cin >> courierNum;
            }

            if (courierNum == "#") {
                send(cliSock, courierNum.c_str(), 1, 0);
                break;
            }
            unsigned int num = stoul(courierNum);
            send(cliSock, (char*)&num, 4, 0);

            recv(cliSock, &msg, 1, 0);
            if (msg == ITEM_NOT_BELONG_TO_YOU) {
                cout << "You can not pick up this express" << endl;
            }
            else if (msg == EXPRESS_ALREADY_PICKUP) {
                cout << "This express is already picked up" << endl;
            }
            else if (msg == ELEMENT_NOT_FOUND) {
                cout << "No such express" << endl;
            }
            else {
                i++;
            }
        }
    }
    else if (command == commands.at(6) && !ban.contains(6)) {
        outBuf << DSPLYNPEXP;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        cout << "Not picked-up expresses:" << endl;
        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == CONTAINER_EMPTY) {
            cout << "No not-picked-up express" << endl;
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
    else if (command == commands.at(7) && !ban.contains(7)) {
        outBuf << DSPLYNREXP;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        cout << "Not received expresses:" << endl;
        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == CONTAINER_EMPTY) {
            cout << "No not-received express" << endl;
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
    else if (command == commands.at(8) && !ban.contains(8)) {
        outBuf << DSPLYREXP;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        cout << "Received expresses:" << endl;
        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == CONTAINER_EMPTY) {
           cout << "No received express" << endl;
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
    else if (command == commands.at(9) && !ban.contains(9)) {
        cout << "Please input the sender's username: ";
        string sender;
        cin >> sender;
        outBuf << SRCHSENDER << sender << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == ELEMENT_NOT_FOUND) {
            cout << "No express sended by " << sender << endl;
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
    else if (command == commands.at(10) && !ban.contains(10)) {
        cout << "Please input the receiver's username: ";
        string receiver;
        cin >> receiver;
        outBuf << SRCHRECEIVER << receiver << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == ELEMENT_NOT_FOUND) {
            cout << "No express received by " << receiver << endl;
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
    else if (command == commands.at(11) && !ban.contains(11)) {
        cout << "Please input the time information in format %Y-%m-%d %H:%M:%S" << endl;
        cout << "Lower Bound: ";
        char lowerBound[20];
        cin.getline(lowerBound, 20);
        cin .getline(lowerBound, 20);
        cout << "Upper Bound: ";     
        char upperBound[20];
        cin.getline(upperBound, 20);

        outBuf << SRCHPICKTM << lowerBound << endl << upperBound << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == ELEMENT_NOT_FOUND) {
            cout << "No express picked up between " << lowerBound << "and " << upperBound << endl;
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
    else if (command == commands.at(12) && !ban.contains(12)) {
        cout << "Please input the courier number: ";
        string courierNum;
        cin >> courierNum;
        regex r("[0-9]{10}");
        while (!regex_match(courierNum, r)) {
            cout << "Format error, courier number has 10 digits" << endl;
            cout << "Please input the courier number: ";
            cin >> courierNum;
        }
        
        outBuf << FINDEXP << courierNum << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == ELEMENT_NOT_FOUND) {
            cout << "No express whose courier number is " << courierNum << endl;
            return false;
        }

        char recvBuf[MAX_BUFFER_SIZE];
        int len = recv(cliSock, recvBuf, MAX_BUFFER_SIZE, 0);
        recvBuf[len] = 0;
        cout << recvBuf << endl;

    }
    else if (command == commands.at(13) && !ban.contains(13)) {
        cout << "Available commands:" << endl;
        if (!ban.contains(0))
            cout << "  " << commands.at(0) << ": return to the upper module" << endl;
        if (!ban.contains(1))
            cout << "  " << commands.at(1) << ": check your information" << endl;
        if (!ban.contains(2))
            cout << "  " << commands.at(2) << ": change your password" << endl;
        if (!ban.contains(3))
            cout << "  " << commands.at(3) << ": change your name" << endl;
        if (!ban.contains(4))
            cout << "  " << commands.at(4) << ": change your phone number" << endl;
        if (!ban.contains(5))
            cout << "  " << commands.at(5) << ": pick up expresses" << endl;
        if (!ban.contains(6))
            cout << "  " << commands.at(6) << ": display all not picked-up expresses' message" << endl;
        if (!ban.contains(7))
            cout << "  " << commands.at(7) << ": display all not received expresses' message" << endl;
        if (!ban.contains(8))
            cout << "  " << commands.at(8) << ": display all received expresses' message" << endl;
        if (!ban.contains(9))
            cout << "  " << commands.at(9) << ": search the related expresses by sender" << endl;
        if (!ban.contains(10))
            cout << "  " << commands.at(10) << ": search the related expresses by receiver" << endl;
        if (!ban.contains(11))
            cout << "  " << commands.at(11) << ": search the related expresses by pick-up time" << endl;
        if (!ban.contains(12))
            cout << "  " << commands.at(12) << ": find the specific expresses by courier number" << endl;
        if (!ban.contains(13))
            cout << "  " << commands.at(13) << ": print this help" << endl;
    }
    else
        cout << "Unknown command" << endl;
    return false;
}
