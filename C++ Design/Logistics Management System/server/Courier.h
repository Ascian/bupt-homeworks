#pragma once

#include<set>
#include<map>

#include"Express.h"
#include "Account.h"

class Courier :
    public Account{
private:
    set<Express*> notPickedList;//�����տ��
    set<Express*> notReceivedList;//��ǩ�տ��
    set<Express*> receivedList;//��ǩ�տ��
    multimap<time_t, Express*> pickTimeList;//���տ��ʱ����ұ�
    multimap<string, Express*> receiverList;//���ͷ����ұ�
    multimap<string, Express*> senderList;//���շ����ұ�
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

    //չʾ���д����տ����Ϣ
    const bool notPExpToString(ostringstream& oss, const int begin, const int end);

    //չʾ���д�ǩ�տ����Ϣ
    const bool notRExpToString(ostringstream& oss, const int begin, const int end);

    //չʾ������ǩ�տ����Ϣ
    const bool rExpToString(ostringstream& oss, const int begin, const int end);

    //��������ʱ���ѯ���
    const vector<const Express*> searchPickTime(const time_t& begin, const time_t end);;

    //���ݷ����˲��ҿ��
    const vector<const Express*> searchSender(const string& sender);

    //���ݽ����˲��ҿ��
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

