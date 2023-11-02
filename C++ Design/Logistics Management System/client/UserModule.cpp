#include "UserModule.h"

bool UserModule::execute(SOCKET& cliSock, const string& command, const set<int>& ban)
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
        cout << "Please input your new address: ";
        string address;
        cin >> address;
        while (address.size() > STRING_MAX_SIZE) {
            cout << "Address length should be less than " << STRING_MAX_SIZE << endl;
            cout << "Please input your new address: ";
            cin >> address;
        }
        outBuf << ADDRESS << address << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
    }
    else if (command == commands.at(6) && !ban.contains(6)) {
        cout << "Please input the amount of money you want to recharge: ";
        string money;
        cin >> money;
        regex r("^\\d+$");
        while (!regex_match(money, r) || money.length() > 9 ) {
            if (!regex_match(money, r))
                cout << "Format error, please input a positive integer" << endl;
            else
                cout << "The amount of money exceeds the limit" << endl;
            cout << "Please input the amount of money you want to recharge: ";
            cin >> money;
        }
        outBuf << RECHARGE << money << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
    }
    else if (command == commands.at(7) && !ban.contains(7)) {
        cout << "Please input the express information" << endl;
        cout << "Receiver's username: ";
        string receiver;
        cin >> receiver;
        cout << "Condition: ";
        string condition;
        cin >> condition;
        cout << "Description: ";
        string description;
        cin >> description;
        cout << "Kind(";
        for (int i = 0; i < Express_subKind().size() - 1; i++) {
            cout << Express_subKind().at(i) << "/";
        }
        cout << Express_subKind().at(Express_subKind().size() - 1) << "): ";
        string kind;
        cin >> kind;
        cout << "Amount: ";
        string amount;
        cin >> amount;
        regex r("^\\d+$");
        while (!regex_match(amount, r) || amount.length() > 9 ) {
            if (!regex_match(amount, r))
                cout << "Format error, please input a positive integer" << endl;
            else
                cout << "The amount exceeds the limit" << endl;
            cout << "Amount: ";
            cin >> amount;
        }

        outBuf << SEND << receiver << endl << condition << endl << description << endl << 
            kind << endl << amount << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == SUCCESS) {
            cout << "Succeed to send an express" << endl;
        }
        else if (msg == RECEIVER_EQUAL_SENDER) {
            cout << "You can not send an express to yourself" << endl;
        }
        else if (msg == RECEIVER_NOT_EXIST) {
            cout << "Receiver does not exist" << endl;
        }
        else if(msg == ELEMENT_EXCEED_LIMIT) {
            cout << "Express amount exceeds the limit" << endl;
        }
        else if(msg == EXPRESS_UNKNOWN_KIND) {
            cout << "Unknown kind" << endl;
        }
        else {
            cout << "Balance not enough" << endl;
        }
    }
    else if (command == commands.at(8) && !ban.contains(8)) {
        outBuf << SIGNFOR;
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

        cout << "Please input the courier numbers of the expresses you want to sign for" << endl;
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
                cout << "You can not sign for this express" << endl;
            }
            else if (msg == EXPRESS_ALREADY_SIGNFOR) {
                cout << "This express is already signed for" << endl;
            }
            else if (msg == ELEMENT_NOT_FOUND) {
                cout << "No such express" << endl;
            }
            else if (msg == EXPRESS_FORBIT_SIGNFOR) {
                cout << "This express is not sended yet" << endl;
            }
            else {
                i++;
            }
        }
    }
    else if (command == commands.at(9) && !ban.contains(9)) {
        outBuf << SENDINFRM;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        currentModule = 0;
    }
    else if (command == commands.at(10) && !ban.contains(10)) {
        outBuf << RECEIVEINFRM;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        currentModule = 1;
    }
    else if (command == commands.at(11) && !ban.contains(11)) {
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
    else if (command == commands.at(12) && !ban.contains(12)) {
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
            cout << "  " << commands.at(5) << ": change your address" << endl;
        if (!ban.contains(6))
            cout << "  " << commands.at(6) << ": recharge your balance" << endl;
        if (!ban.contains(7))
            cout << "  " << commands.at(7) << ": send an express" << endl;
        if (!ban.contains(8))
            cout << "  " << commands.at(8) << ": sign for expresses" << endl;
        if (!ban.contains(9))
            cout << "  " << commands.at(9) << ": enter the sending information module" << endl;
        if (!ban.contains(10))
            cout << "  " << commands.at(10) << ": enter the receiving information module" << endl;
        if (!ban.contains(11))
            cout << "  " << commands.at(11) << ": find the specific expresses by courier number" << endl;
        if (!ban.contains(12))
            cout << "  " << commands.at(12) << ": print this help" << endl;
    }
    else
        cout << "Unknown command" << endl;
    return false;
}
