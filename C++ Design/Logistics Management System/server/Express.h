#pragma once
#include<ctime>
#include<string>
#include<sstream>
#include<iostream>
#include<fstream>
#include<iomanip>
#include<vector>

using namespace std;

enum state{NOT_ASSIGNED, NOT_SENDED, NOT_RECEIVED};

class Express{
protected:
	string courier;//快递员
	string sender;//发送方
	string receiver;//接受方
	time_t creatTime;//创建时间
	time_t sendTime;//发送时间，NOT_DESTRIBUTED表示未分配快递员，NOT_SENDED表示未揽收
	time_t receiveTime;//签收时间，NOT_RECEIVED表示未签收
	string condition;//物品状态
	string description;//物品描述
	unsigned int amount; //数量
	unsigned int courierNumber;//快递单号
public:
	Express() { 
		creatTime = 0;
		sendTime = NOT_ASSIGNED;
		receiveTime = NOT_RECEIVED;
		amount = 0;
		courierNumber = UINT_MAX;
	}

	Express(const string& sender, const string& receiver, const time_t creatTime,
		const string& condition, const string& description, const unsigned int courierNumber,
		const unsigned int amount)
	:courier("---NONE---"), sender(sender), receiver(receiver), condition(condition), description(description) {
		this->creatTime = creatTime;
		sendTime = NOT_ASSIGNED;
		receiveTime = NOT_RECEIVED;
		this->amount = amount;
		this->courierNumber = courierNumber;
	}

	inline static const vector<string> subKind() {
		return { "normal", "fragile", "book" };
	}	

	inline void setCourier(const string& courier) { this->courier = courier; }

	inline void setSender(const string& sender) { this->sender = sender; }

	inline void setReceiver(const string& receiver) { this->receiver = receiver; }

	inline void serCreatTime(const time_t& creatTime) { this->creatTime = creatTime; }

	inline void setSendTime(const time_t& sendTime) { this->sendTime = sendTime; }

	inline void setReceiveTime(const time_t& receiveTime) { this->receiveTime = receiveTime; }

	inline void setCondition(const string& condition) { this->condition = condition; }

	inline void setDescription(const string& description) { this->description = description; }
	
	inline void setAmount(const unsigned int amount) { this->amount = amount; }

	inline void setCourierNumber(const unsigned int courierNumber) { this->courierNumber = courierNumber; }

	inline const string& getCourier() { return courier; }

	inline const string& getSender() { return sender; }

	inline const string& getReceiver() { return receiver; }

	inline const time_t& getCreatTime() { return creatTime; }

	inline const time_t& getSendTime() { return sendTime; }

	inline const time_t& getReceiveTime() { return receiveTime; }

	inline const string& getCondition() { return condition; }

	inline const string& getDescription() { return description; }

	inline const unsigned int getAmount() { return amount; }

	inline const unsigned int getCourierNumber() { return courierNumber; }

	//计算获得快递价格
	inline virtual const unsigned int getPrice() = 0;

	//获得快递类型
	inline virtual const string getKind() = 0;

	friend inline ostream& operator<<(ostream& Ostr, Express& express);

	friend inline istream& operator>>(istream& Istr, Express& express);

};

inline ostream& operator<<(ostream& Ostr, Express& express) {
	Ostr << "Kind: " << express.getKind() << endl <<
		"Courier Number: " << setfill('0') << setw(10) << express.courierNumber << endl <<
		"Courier: " << express.courier << endl <<
		"Sender: " << express.sender << endl <<
		"Reseiver: " << express.receiver << endl <<
		"Creat Time: " << put_time(localtime(&express.creatTime), "%Y-%b-%d %H:%M:%S") << endl;

	if (express.sendTime == NOT_ASSIGNED)//未分配快递员
		Ostr << "Send Time: " << "Not assgined to a courier" << endl;
	else if (express.sendTime == NOT_SENDED) {
		Ostr << "Send Time: " << "Not picked up" << endl;
	}
	else
		Ostr << "Send Time: " << put_time(localtime(&express.sendTime), "%Y-%b-%d %H:%M:%S") << endl;

	if (express.receiveTime == NOT_RECEIVED)//未签收
		Ostr << "Receive Time: " << "Not signed for" << endl;
	else
		Ostr << "Receive Time: " << put_time(localtime(&express.receiveTime), "%Y-%b-%d %H:%M:%S") << endl;

	return Ostr << "Condition: " << express.condition << endl <<
		"Description: " << express.description << endl <<
		"Amount: " << express.amount << endl;
}

inline istream& operator>>(istream& Istr, Express& express)
{
	Istr.ignore(strlen("Courier Number: "));
	Istr >> express.courierNumber;
	Istr.ignore(strlen("Courier: "));
	Istr >> express.courier;
	Istr.ignore(strlen("Sender: "));
	Istr >> express.sender;
	Istr.ignore(strlen("Reseiver: "));
	Istr >> express.receiver;
	Istr.ignore(strlen("Creat Time: "));
	struct tm time;
	Istr >> get_time(&time, "%Y-%b-%d %H:%M:%S");
	express.creatTime = mktime(&time);

	Istr.ignore(strlen("Send Time: "));
	char temp[30];
	Istr.getline(temp, 30);
	if (!strcmp(temp, " Not assgined to a courier"))
		express.sendTime = NOT_ASSIGNED;
	else if(!strcmp(temp, " Not picked up"))
		express.sendTime = NOT_SENDED;
	else {
		istringstream str(temp);
		str >> get_time(&time, "%Y-%b-%d %H:%M:%S");
		express.sendTime = mktime(&time);
	}

	Istr.ignore(strlen("Receive Time: "));
	Istr.getline(temp, 30);
	if (!strcmp(temp, "Not signed for"))
		express.receiveTime = NOT_RECEIVED;
	else {
		istringstream str(temp);
		str >> get_time(&time, "%Y-%b-%d %H:%M:%S");
		express.receiveTime = mktime(&time);
	}

	Istr.ignore(strlen("Condition: "));
	Istr >> express.condition;
	Istr.ignore(strlen("Description: "));
	Istr >> express.description;
	Istr.ignore(strlen("Amount: "));
	Istr >> express.amount;
	return Istr;
}
