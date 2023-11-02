#pragma once
#include"User.h"
#include"Manager.h"
#include"Express.h"
#include"Fragile.h"
#include"NormalExpress.h"
#include"Book.h"
#include"Courier.h"
#include"Event.h"

#include<ctime>
#include<map>
#include<string>
#include<iostream>
#include<fstream>

using namespace std;

class Logistics{
private:
	map<string, Courier*> couriers;//快递员查找表
	map<string, User*> users;//用户名查找表
	map<unsigned int, Express*> expresses;//快递单号查找表	
	map<time_t, Express*> createTimeList; //物流创建时间查找表
	set<Express*> notAssignedList; //未分配快递员
public:
	Manager manager;//管理员信息

	Logistics(){}

	~Logistics() {
		for (auto& courier : couriers)
			delete(courier.second);
		for (auto& user : users)
			delete(user.second);
		for (auto& express : expresses)
			delete(express.second);
	}

	//注册账号
	bool createUserAccount(const string& username, const string& password, const string& name,
		const unsigned _int64 phone, const string& address);

	//创建快递员账号
	bool createCourierAccount(const string& username, const string& password, const string& name,
		const unsigned _int64 phone);

	//登录管理员账号
	bool loginManagerAccount(const string& username, const string& password);

	//登录用户账号
	User* loginUserAccount(const string& username,const string& password);

	//登录快递员账号
	Courier* loginCourierAccount(const string& username, const string& password);
	
	//删除快递员账号
	bool deleteCourierAccount(const string& username);

	//创建一个物流快递
	unsigned int createLogistics(const string& senderUsername, const string& receiverUsername,
		const string& condition, const string& description, const unsigned int amount,
		const string& kind);

	//待揽收快递分配快递员
	bool assignExpress(Express* pExpress, const string& courierUsername);

	//快递员揽收快递
	void pickUpExpress(Express* pExpress);

	//用户签收快递
	void signForExpress(Express* pExpress);

	//输出所有用户信息
	const bool usersToString(ostringstream& oss, const int begin, const int end);

	//输出所有快递员信息
	const bool couriersToString(ostringstream& oss, const int begin, const int end);

	//输出所有快递信息
	const bool expressesToString(ostringstream& oss, const int begin, const int end);

	//展示所有未分配快递员的快递信息
	const bool notAExpToString(ostringstream& oss, const int begin, const int end);

	inline unsigned int notAssignedListSize() { return notAssignedList.size(); }

	//根据快递员用户名查询用户
	Courier* findCourier(const string& username);

	//根据用户名查询用户
	User* findUser(const string& username);

	//根据快递单号查询快递
	Express* findExpress(const unsigned int courierNumber);

	//根据创建时间查询快递信息
	const vector<const Express*> searchCreateTime(const time_t& begin, const time_t end);

	//保存数据到savePath
	void save(const string& savePath);

	//从loadPath中读取数据
	void load(const string& loadPath);
};
