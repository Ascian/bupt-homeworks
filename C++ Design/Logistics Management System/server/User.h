#pragma once
#include "Account.h"
#include "Express.h"

#include<ctime>
#include<set>
#include<map>

using namespace std;

class User :
    public Account {
private:
    string address;//地址

    //发送方
    set<Express*> notSendedList;//未发出快递
    multimap<time_t, Express*> sendTimeList;//发出快递时间查找表
    multimap<string, Express*> senderList;//接收方查找表

    //接收方
    set<Express*> notPickedList; //未揽收快递
    set<Express*> notReceivedList;//未签收快递
    multimap<time_t, Express*> receiveTimeList;//收到快递时间查找表
    multimap<string, Express*> receiverList;//发送方查找表
public:
    User() {}

    User(const string& username, const string& password, const string& name,
        const unsigned _int64 phone, const string& address)
        :Account(username, password, name, phone), address(address) {
    }

    inline void setAddress(const string& address) { this->address = address; }

    inline const string& getAddress() { return address; }

    inline void addNotSendedList(Express* pExpress) { notSendedList.insert(pExpress); }

    inline void deleteNotSendedList(Express* pExpress) { notSendedList.erase(pExpress); }

    inline void addNotReceivedList(Express* pExpress) { notReceivedList.insert(pExpress); }

    inline void deleteNotReceivedList(Express* pExpress) { notReceivedList.erase(pExpress); }

    inline void addNotPickedList(Express* pExpress) { notPickedList.insert(pExpress); }

    inline void deleteNotPickedList(Express* pExpress) { notPickedList.erase(pExpress); }

    inline void addSendList(Express* pExpress) { sendTimeList.insert({ pExpress->getSendTime(),pExpress }); }

    inline void addReceiveList(Express* pExpress) { receiveTimeList.insert({ pExpress->getReceiveTime(),pExpress }); }

    inline void addSenderList(Express* pExpress) { senderList.insert({ pExpress->getSender(),pExpress }); }

    inline void addReceiverList(Express* pExpress) { receiverList.insert({ pExpress->getReceiver(),pExpress }); }

    inline unsigned int notReceivedListSize() { return notReceivedList.size(); }

    //展示所有未发出快递信息
    const bool notSExpToString(ostringstream& oss, const int begin, const int end);

    //展示所有发出快递信息
    const bool sExpToString(ostringstream& oss, const int begin, const int end);

    //展示所有未揽收快递信息
    const bool notPExpToString(ostringstream& oss, const int begin, const int end);

    //展示所有未签收快递信息
    const bool notRExpToString(ostringstream& oss, const int begin, const int end);

    //展示所有收到快递信息
    const bool rExpToString(ostringstream& oss, const int begin, const int end);

    //根据发出时间查询快递
    const vector<const Express*> searchSendTime(const time_t& begin, const time_t end);

    //根据接收时间查询快递
    const vector<const Express*> searchReceiveTime(const time_t& begin, const time_t end);

    //根据发送人查找快递
    const vector<const Express*> searchSender(const string& sender);

    //根据接收人查找快递
    const vector<const Express*> searchReceiver(const string& receiver);

    friend inline ostream& operator<<(ostream& Ostr, const User& user);

    friend inline istream& operator>>(istream& Istr, User& user);

};
inline ostream& operator<<(ostream& Ostr, const User& user) {
    Ostr << (Account)user;
    return Ostr << "Address: " << user.address << endl;
}

inline istream& operator>>(istream& Istr, User& user) {
    Istr >> (Account&)user;
    Istr.ignore(strlen("Address: "));
    Istr >> user.address;
    return Istr;
}