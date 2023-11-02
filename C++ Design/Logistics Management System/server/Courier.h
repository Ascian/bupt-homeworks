#pragma once

#include<set>
#include<map>

#include"Express.h"
#include "Account.h"

class Courier :
    public Account{
private:
    set<Express*> notPickedList;//待揽收快递
    set<Express*> notReceivedList;//待签收快递
    set<Express*> receivedList;//已签收快递
    multimap<time_t, Express*> pickTimeList;//揽收快递时间查找表
    multimap<string, Express*> receiverList;//发送方查找表
    multimap<string, Express*> senderList;//接收方查找表
public:
    Courier(){}

    Courier(const string& username, const string& password, const string& name, const unsigned _int64 phone)
        :Account(username, password, name,phone) {}

    inline void addNotPickedList(Express* pExpress) { notPickedList.insert(pExpress); }

    inline void deleteNotPickedList(Express* pExpress) { notPickedList.erase(pExpress); }

    inline void addNotReceivedList(Express* pExpress) { notReceivedList.insert(pExpress); }

    inline void deleteNotReceivedList(Express* pExpress) { notReceivedList.erase(pExpress); }

    inline void addReceivedList(Express* pExpress) { receivedList.insert(pExpress); }

    inline void deleteReceivedList(Express* pExpress) { receivedList.erase(pExpress); }

    inline void addPickTimeList(Express* pExpress) { pickTimeList.insert({ pExpress->getSendTime(),pExpress }); }

    inline void addSenderList(Express* pExpress) { senderList.insert({ pExpress->getSender(),pExpress }); }

    inline void addReceiverList(Express* pExpress) { receiverList.insert({ pExpress->getReceiver(),pExpress }); }

    inline unsigned int notPickedListSize() { return notPickedList.size(); }

    //展示所有待揽收快递信息
    const bool notPExpToString(ostringstream& oss, const int begin, const int end);

    //展示所有待签收快递信息
    const bool notRExpToString(ostringstream& oss, const int begin, const int end);

    //展示所有已签收快递信息
    const bool rExpToString(ostringstream& oss, const int begin, const int end);

    //根据揽收时间查询快递
    const vector<const Express*> searchPickTime(const time_t& begin, const time_t end);;

    //根据发送人查找快递
    const vector<const Express*> searchSender(const string& sender);

    //根据接收人查找快递
    const vector<const Express*> searchReceiver(const string& receiver);

    friend inline ostream& operator<<(ostream& Ostr, const Courier& courier);

    friend inline istream& operator>>(istream& Istr, Courier& courier);

};
inline ostream& operator<<(ostream& Ostr, const Courier& courier) {
    Ostr << (Account)courier;
    return Ostr;
}

inline istream& operator>>(istream& Istr, Courier& courier) {
    Istr >> (Account&)courier;
    return Istr;
}

