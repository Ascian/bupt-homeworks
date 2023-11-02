#pragma once
#include "Account.h"
#include<fstream>

class Manager :
    public Account
{
public:
    Manager(){}

    Manager(const string& username, const string& password, const string& name)
        :Account(username,password,name,0){}

    friend inline ostream& operator<<(ostream& Ostr, const Manager& manager);

    friend inline istream& operator>>(istream& Istr, Manager& manager);

};
inline ostream& operator<<(ostream& Ostr, const Manager& manager) {
    Ostr << (Account)manager;
    return Ostr;
}

inline istream& operator>>(istream& Istr, Manager& manager) {
    Istr >> (Account&)manager;
    return Istr;
}

