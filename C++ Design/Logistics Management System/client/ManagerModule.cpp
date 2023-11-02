#include "ManagerModule.h"

bool ManagerModule::execute(SOCKET& cliSock, const string& command, const set<int>& ban)
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
        cout << "Please input the new password: ";
        string password;
        cin >> password;
        while (password.size() > STRING_MAX_SIZE) {
            cout << "Password length should be less than " << STRING_MAX_SIZE << endl;
            cout << "Please input the new password: ";
            cin >> password;
        }
        outBuf << PASSWORD << password << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
    }
    else if (command == commands.at(3) && !ban.contains(3)) {
        cout << "Please input the new name: ";
        string name;
        cin >> name;
        while (name.size() > STRING_MAX_SIZE) {
            cout << "Name length should be less than " << STRING_MAX_SIZE << endl;
            cout << "Please input the new name: ";
            cin >> name;
        }
        outBuf << NAME << name << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
    }
    else if (command == commands.at(4) && !ban.contains(4)) {
        cout << "Please input the account information" << endl;
        string username;
        string password;
        string name;
        string phone;
        cout << "Username: ";
        cin >> username;
        while (username.size() > STRING_MAX_SIZE) {
            cout << "Username length should be less than " << STRING_MAX_SIZE << endl;
            cout << "Username: ";
            cin >> username;
        }
        cout << "Password: ";
        cin >> password;
        while (password.size() > STRING_MAX_SIZE) {
            cout << "Password length should be less than " << STRING_MAX_SIZE << endl;
            cout << "Password: ";
            cin >> password;
        }
        cout << "Name: ";
        cin >> name;
        while (name.size() > STRING_MAX_SIZE) {
            cout << "Name length should be less than " << STRING_MAX_SIZE << endl;
            cout << "Name: ";
            cin >> name;
        }
        cout << "Phone Number: ";
        cin >> phone;
        regex r("^\\d+$");
        while (!regex_match(phone, r) || phone.length() > 11) {
            cout << "Format error, phone number consists of up to 11 digits" << endl;
            cout << "Phone Number: ";
            cin >> phone;
        }

        outBuf << ADDCOURIER << username << endl << password << endl << name << endl <<
            phone << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == SUCCESS) {
            cout << "Succeed to add courier" << endl;
        }
        else if (msg == ELEMENT_EXCEED_LIMIT) {
            cout << "Username amount reached the Maximum" << endl;
        }
        else {
            cout << "Username already exists" << endl;
        }
    }
    else if (command == commands.at(5) && !ban.contains(5)) {
        cout << "Please input the courier's username: ";
        string username;
        cin >> username;
        outBuf << DELETECOURIER << username << endl ;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == SUCCESS) {
            cout << "Succeed to delete courier" << endl;
        }
        else {
            cout << "No such courier" << endl;
        }
    }
    else if (command == commands.at(6) && !ban.contains(6)) {
        cout << "Courier's username: ";
        string username;
        cin >> username;
        outBuf << ASSIGN << username << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        cout << "Not assigned expresses:" << endl;
        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == CONTAINER_EMPTY) {
            cout << "No not-assigned express" << endl;
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

        cout << "Please input the information of the express and the courier" << endl;
        cout << "Express's courier number: ";
        string courierNum;
        cin >> courierNum;
        regex r("[0-9]{10}");
        while (!regex_match(courierNum, r)) {
            cout << "Format error, courier number has 10 digits" << endl;
            cout << "Express's courier number: ";
            cin >> courierNum;
        }

        unsigned int num = stoul(courierNum);
        send(cliSock, (char*)&num, 4, 0);

        msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == SUCCESS) {
            cout << "Succeed to assign" << endl;
        }
        else if(msg == ELEMENT_NOT_FOUND) {
            cout << "No such express or courier" << endl;
        }
        else {
            cout << "The express is already assgined to a courier" << endl;
        }
    }
    else if (command == commands.at(7) && !ban.contains(7)) {
        outBuf << DSPLYUSER;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == CONTAINER_EMPTY) {
           cout << "No user" << endl;
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
        outBuf << DSPLYCOURIER;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == CONTAINER_EMPTY) {
           cout << "No courier" << endl;
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
        outBuf << DSPLYEXP;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == CONTAINER_EMPTY) {
           cout << "No express" << endl;
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
        cout << "Please input the time information in format %Y-%m-%d %H:%M:%S" << endl;
        cout << "Lower Bound: ";
        char lowerBound[20];
        cin.getline(lowerBound, 20);
        cin .getline(lowerBound, 20);
        cout << "Upper Bound: ";     
        char upperBound[20];
        cin .getline(upperBound, 20);

        outBuf << SRCHCREATETM << lowerBound << endl << upperBound << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == ELEMENT_NOT_FOUND) {
            cout << "No express created between " << lowerBound << "and " << upperBound << endl;
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
        string username;
        cout << "Username: ";
        cin >> username;
        while (username.size() > STRING_MAX_SIZE) {
            cout << "Username length should be less than " << STRING_MAX_SIZE << endl;
            cout << "Username: ";
            cin >> username;
        }

        outBuf << FINDUSER << username << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == SUCCESS) {
            currentModule = 0;
        }
        else {
            cout << "Username not found" << endl;
        }
    }
    else if (command == commands.at(12) && !ban.contains(12)) {
        string username;
        cout << "Username: ";
        cin >> username;
        while (username.size() > STRING_MAX_SIZE) {
            cout << "Username length should be less than " << STRING_MAX_SIZE << endl;
            cout << "Username: ";
            cin >> username;
        }

        outBuf << FINDCOURIER << username << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == SUCCESS) {
            currentModule = 1;
        }
        else {
            cout << "Username not found" << endl;
        }
    }
    else if (command == commands.at(13) && !ban.contains(13)) {
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
    else if (command == commands.at(14) && !ban.contains(14)) {
        cout << "Available commands:" << endl;
        if (!ban.contains(0))
            cout << "  " << commands.at(0) << ": return to the upper module" << endl;
        if (!ban.contains(1))
            cout << "  " << commands.at(1) << ": check the manager information" << endl;
        if (!ban.contains(2))
            cout << "  " << commands.at(2) << ": change the password" << endl;
        if (!ban.contains(3))
            cout << "  " << commands.at(3) << ": change the name" << endl;
        if (!ban.contains(4))
            cout << "  " << commands.at(4) << ": add a new courier account" << endl;
        if (!ban.contains(5))
            cout << "  " << commands.at(5) << ": delete a courier account" << endl;
        if (!ban.contains(6))
            cout << "  " << commands.at(6) << ": assign an express to a courier" << endl;
        if (!ban.contains(7))
            cout << "  " << commands.at(7) << ": display all the users' massage" << endl;
        if (!ban.contains(8))
            cout << "  " << commands.at(8) << ": display all the couriers' massage" << endl;
        if (!ban.contains(9))
            cout << "  " << commands.at(9) << ": display all the expresses' massage" << endl;
        if (!ban.contains(10))
            cout << "  " << commands.at(10) << ": search the related expresses by creating time" << endl;
        if (!ban.contains(11))
            cout << "  " << commands.at(11) << ": find the specific user and enter the user module" << endl;
        if (!ban.contains(12))
            cout << "  " << commands.at(12) << ": find the specific courier and enter the courier module" << endl;
        if (!ban.contains(13))
            cout << "  " << commands.at(13) << ": find the specific express by courier number" << endl;
        if (!ban.contains(14))
            cout << "  " << commands.at(14) << ": print this help" << endl;
    }
    else
        cout << "Unknown command" << endl;
    return false;
}
