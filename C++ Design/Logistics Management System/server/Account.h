#pragma once

#include<string>
#include<iostream>

#include"Event.h"

using namespace std;

class Account{
protected:
	string username;//用户名
	string password;//密码
	string name;//姓名
	double balance;//余额
	unsigned _int64 phone;//电话
public:
	Account() {
		balance = 0;
		phone = 0;
	}
	Account(const string& username, const string& password,const string& name, const unsigned _int64 phone)
	:username(username), password(password) ,name(name) {
		balance = 0;
		this->phone = phone;
	}

	inline void setUsername(const string& username) { this->username = username; }

	inline void setPassword(const string& password) { this->password = password; }

	inline void setName(const string& name) { this->name = name; }

	inline void setPhone(const unsigned _int64 phone) { this->phone = phone; }

	inline void setBalance(const double balance) { this->balance = balance; }

	inline const string& getUsername() { return username; }

	inline const string& getPassword() {return password;}

	inline const string& getName() { return name; }

	inline const unsigned _int64 getPhone() { return phone; }

	inline const double getBalance() { return balance; }

	inline void subBalance(const double amount) { balance -= amount; }//扣除

	inline void addBalance(const double amount) { balance += amount; }//进账

	friend inline ostream& operator<<(ostream& Ostr, const Account& account);

	friend inline istream& operator>>(istream& Istr, Account& account);

};
inline ostream& operator<<(ostream& Ostr, const Account& account) {
	return Ostr << "Username: " << account.username << endl <<
		"Password: " << account.password << endl <<
		"Name: " << account.name << endl <<
		"Balance: " << account.balance << endl <<
		"Phone Number: " << account.phone << endl ;
}

inline istream& operator>>(istream& Istr, Account& account) {
	Istr.ignore(strlen("Username: "));
	Istr >> account.username;
	Istr.ignore(strlen("Password: "));
	Istr >> account.password;
	Istr.ignore(strlen("Name: "));
	Istr >> account.name;
	Istr.ignore(strlen("Balance: "));
	Istr >> account.balance;
	Istr.ignore(strlen("Phone Number: "));
	Istr >> account.phone;
	return Istr;
}
