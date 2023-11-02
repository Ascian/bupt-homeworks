#include "User.h"

const bool User::sExpToString(ostringstream& oss, const int begin, const int end) {
    if (sendTimeList.size() == 0) {
        throw(char)(CONTAINER_EMPTY);
        return false;
    }
    unsigned int i = 1;
    auto iter = sendTimeList.begin();
    for (; i < begin && iter != sendTimeList.end(); i++, iter++);
    for (; i <= end && iter != sendTimeList.end(); i++, iter++) {
        oss << "[" << i << "]" << endl;
        oss << *(iter->second) << endl;
    }
    if (iter != sendTimeList.end()) {
        return true;
    }
    return false;
}

const bool User::notPExpToString(ostringstream& oss, const int begin, const int end)
{
    if (notPickedList.size() == 0) {
        throw(char)(CONTAINER_EMPTY);
        return false;
    }
    unsigned int i = 1;
    auto iter = notPickedList.begin();
    for (; i < begin && iter != notPickedList.end(); i++, iter++);
    for (; i <= end && iter != notPickedList.end(); i++, iter++) {
        oss << "[" << i << "]" << endl;
        oss << **iter << endl;
    }
    if (iter != notPickedList.end()) {
        return true;
    }
    return false;
}

const bool User::rExpToString(ostringstream& oss, const int begin, const int end) {
    if (receiveTimeList.size() == 0) {
        throw(char)(CONTAINER_EMPTY);
        return false;
    }
    unsigned int i = 1;
    auto iter = receiveTimeList.begin();
    for (; i < begin && iter != receiveTimeList.end(); i++, iter++);
    for (; i <= end && iter != receiveTimeList.end(); i++, iter++) {
        oss << "[" << i << "]" << endl;
        oss << *(iter->second) << endl;
    }
    if (iter != receiveTimeList.end()) {
        return true;
    }
    return false;
}

const bool User::notSExpToString(ostringstream& oss, const int begin, const int end)
{
    if (notSendedList.size() == 0) {
        throw(char)(CONTAINER_EMPTY);
        return false;
    }
    unsigned int i = 1;
    auto iter = notSendedList.begin();
    for (; i < begin && iter != notSendedList.end(); i++, iter++);
    for (; i <= end && iter != notSendedList.end(); i++, iter++) {
        oss << "[" << i << "]" << endl;
        oss << **iter << endl;
    }
    if (iter != notSendedList.end()) {
        return true;
    }
    return false;
}

const bool User::notRExpToString(ostringstream& oss, const int begin, const int end) {
    if (notReceivedList.size() == 0) {
        throw(char)(CONTAINER_EMPTY);
        return false;
    }
    unsigned int i = 1;
    auto iter = notReceivedList.begin();
    for (; i < begin && iter != notReceivedList.end(); i++, iter++);
    for (; i <= end && iter != notReceivedList.end(); i++, iter++) {
        oss << "[" << i << "]" << endl;
        oss << **iter << endl;
    }
    if (iter != notReceivedList.end()) {
        return true;
    }
    return false;
}

const vector<const Express*> User::searchSendTime(const time_t& begin, const time_t end) {
    vector<const Express*> result;
    for (auto temp = sendTimeList.lower_bound(begin); temp != sendTimeList.end() && temp->first <= end; temp++) {
        result.push_back(temp->second);
    }
    if (result.size() == 0)
        throw(char)(ELEMENT_NOT_FOUND);
    return result;
}

const vector<const Express*> User::searchReceiveTime(const time_t& begin, const time_t end) {
    vector<const Express*> result;
    for (auto temp = receiveTimeList.lower_bound(begin);temp!= receiveTimeList.end() && temp->first <= end; temp++) {
        result.push_back(temp->second);
    }
    if (result.size() == 0)
        throw(char)(ELEMENT_NOT_FOUND);
    return result;
}

const vector<const Express*> User::searchSender(const string& sender)
{
    vector<const Express*> result;
    auto temp = senderList.equal_range(sender);
    for (auto i = temp.first; i != temp.second; i++) {
        result.push_back(i->second);
    }
    if (result.size() == 0)
        throw(char)(ELEMENT_NOT_FOUND);
    return result;
}

const vector<const Express*> User::searchReceiver(const string& receiver)
{
    vector<const Express*> result;
    auto temp = receiverList.equal_range(receiver);
    for (auto i = temp.first; i != temp.second; i++) {
        result.push_back(i->second);
    }
    if (result.size() == 0)
        throw(char)(ELEMENT_NOT_FOUND);
    return result;
}




