#include "Courier.h"

const bool Courier::notPExpToString(ostringstream& oss, const int begin, const int end) {
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

const bool Courier::notRExpToString(ostringstream& oss, const int begin, const int end) {
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

const bool Courier::rExpToString(ostringstream& oss, const int begin, const int end)
{
    if (receivedList.size() == 0) {
        throw(char)(CONTAINER_EMPTY);
        return false;
    }
    unsigned int i = 1;
    auto iter = receivedList.begin();
    for (; i < begin && iter != receivedList.end(); i++, iter++);
    for (; i <= end && iter != receivedList.end(); i++, iter++) {
        oss << "[" << i << "]" << endl;
        oss << **iter << endl;
    }
    if (iter != receivedList.end()) {
        return true;
    }
    return false;
}

const vector<const Express*> Courier::searchPickTime(const time_t& begin, const time_t end) {
    vector<const Express*> result;
    for (auto temp = pickTimeList.lower_bound(begin); temp != pickTimeList.end() && temp->first <= end; temp++) {
        result.push_back(temp->second);
    }
    if (result.size() == 0)
        throw(char)(ELEMENT_NOT_FOUND);
    return result;
}

const vector<const Express*> Courier::searchSender(const string& sender)
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

const vector<const Express*> Courier::searchReceiver(const string& receiver)
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