#include "MainModule.h"
bool MainModule::execute(SOCKET& cliSock, const string& command, const set<int>& ban)
{
    ostringstream outBuf;
    if (command == commands.at(0) && !ban.contains(0)) {
        outBuf << EXIT;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        return true;
    }     
    else if (command == commands.at(1) && !ban.contains(1)) {
        string username;
        string password;
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

        outBuf << USER << username << endl << password << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == SUCCESS) {
            currentModule = 0;
        }
        else if (msg == ELEMENT_NOT_FOUND) {
            cout << "Username not found" << endl;
        }
        else {
            cout << "Password error" << endl;
        }
    }
    else if (command == commands.at(2) && !ban.contains(2)) {
        string username;
        string password;
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

        outBuf << COURIER << username << endl << password << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == SUCCESS) {
            currentModule = 1;
        }
        else if (msg == ELEMENT_NOT_FOUND) {
            cout << "Username not found" << endl;
        }
        else {
            cout << "Password error" << endl;
        }
    }
    else if (command == commands.at(3) && !ban.contains(3)) {
        string username;
        string password;
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

        outBuf << MANAGER << username << endl << password << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == SUCCESS) {
            currentModule = 2;
        }
        else if (msg == ELEMENT_NOT_FOUND) {
            cout << "Username error" << endl;
        }
        else {
            cout << "Password error" << endl;
        }
    }
    else if (command == commands.at(4) && !ban.contains(4)) {
        cout << "Please input the account information" << endl;
        string username;
        string password;
        string name;
        string phone;
        string address;
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
        cout << "Home Address: ";
        cin >> address;
        while (address.size() > STRING_MAX_SIZE) {
            cout << "Address length should be less than " << STRING_MAX_SIZE << endl;
            cout << "Home Address: ";
            cin >> address;
        }

        outBuf << REGISTER << username << endl << password << endl << name << endl << 
            phone << endl << address << endl;
        send(cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);

        char msg;
        recv(cliSock, &msg, 1, 0);

        if (msg == SUCCESS) {
            cout << "Succeed to register" << endl;
        }
        else if (msg == ELEMENT_EXCEED_LIMIT) {
            cout << "Username amount reached the Maximum" << endl;
        }
        else {
            cout << "Username already exists" << endl;
        }
    }
    else if (command == commands.at(5) && !ban.contains(5)) {
        cout << "Available commands:" << endl;
        if (!ban.contains(0))
            cout << "  " << commands.at(0) << ": exit the program" << endl;
        if (!ban.contains(1))
            cout << "  " << commands.at(1) << ": enter the user module" << endl;
        if (!ban.contains(2))
            cout << "  " << commands.at(2) << ": enter the courier module" << endl;
        if (!ban.contains(3))
            cout << "  " << commands.at(3) << ": enter the manager module" << endl;
        if (!ban.contains(4))
            cout << "  " << commands.at(4) << ": register a new user account" << endl;
        if (!ban.contains(5))
            cout << "  " << commands.at(5) << ": print this help" << endl;
    }
    else
        cout << "Unknown command" << endl;
    return false;
}
