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
    string address;//��ַ

    //���ͷ�
    set<Express*> notSendedList;//δ�������
    multimap<time_t, Express*> sendTimeList;//�������ʱ����ұ�
    multimap<string, Express*> senderList;//���շ����ұ�

    //���շ�
    set<Express*> notPickedList; //δ���տ��
    set<Express*> notReceivedList;//δǩ�տ��
    multimap<time_t, Express*> receiveTimeList;//�յ����ʱ����ұ�
    multimap<string, Express*> receiverList;//���ͷ����ұ�
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

    //չʾ����δ���������Ϣ
    const bool notSExpToString(ostringstream& oss, const int begin, const int end);

    //չʾ���з��������Ϣ
    const bool sExpToString(ostringstream& oss, const int begin, const int end);

    //չʾ����δ���տ����Ϣ
    const bool notPExpToString(ostringstream& oss, const int begin, const int end);

    //չʾ����δǩ�տ����Ϣ
    const bool notRExpToString(ostringstream& oss, const int begin, const int end);

    //չʾ�����յ������Ϣ
    const bool rExpToString(ostringstream& oss, const int begin, const int end);

    //���ݷ���ʱ���ѯ���
    const vector<const Express*> searchSendTime(const time_t& begin, const time_t end);

    //���ݽ���ʱ���ѯ���
    const vector<const Express*> searchReceiveTime(const time_t& begin, const time_t end);

    //���ݷ����˲��ҿ��
    const vector<const Express*> searchSender(const string& sender);

    //���ݽ����˲��ҿ��
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