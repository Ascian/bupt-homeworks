#pragma once

#include"Express.h"

class Book :
    public Express
{ 
public:
    const static int PRICE = 2;

    Book() {}

    Book(const string& sender, const string& receiver, const time_t creatTime,
        const string& condition, const string& description, const unsigned int courierNumber,
        const unsigned int amount)
        :Express(sender, receiver, creatTime, condition, description, courierNumber, amount){}

    inline virtual const unsigned int getPrice() {
        return amount * PRICE;
    }

    inline virtual const string getKind() {
        return Express::subKind().at(2);
    }
};

