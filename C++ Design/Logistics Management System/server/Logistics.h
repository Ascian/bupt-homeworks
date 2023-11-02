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
	map<string, Courier*> couriers;//���Ա���ұ�
	map<string, User*> users;//�û������ұ�
	map<unsigned int, Express*> expresses;//��ݵ��Ų��ұ�	
	map<time_t, Express*> createTimeList; //��������ʱ����ұ�
	set<Express*> notAssignedList; //δ������Ա
public:
	Manager manager;//����Ա��Ϣ

	Logistics(){}

	~Logistics() {
		for (auto& courier : couriers)
			delete(courier.second);
		for (auto& user : users)
			delete(user.second);
		for (auto& express : expresses)
			delete(express.second);
	}

	//ע���˺�
	bool createUserAccount(const string& username, const string& password, const string& name,
		const unsigned _int64 phone, const string& address);

	//�������Ա�˺�
	bool createCourierAccount(const string& username, const string& password, const string& name,
		const unsigned _int64 phone);

	//��¼����Ա�˺�
	bool loginManagerAccount(const string& username, const string& password);

	//��¼�û��˺�
	User* loginUserAccount(const string& username,const string& password);

	//��¼���Ա�˺�
	Courier* loginCourierAccount(const string& username, const string& password);
	
	//ɾ�����Ա�˺�
	bool deleteCourierAccount(const string& username);

	//����һ���������
	unsigned int createLogistics(const string& senderUsername, const string& receiverUsername,
		const string& condition, const string& description, const unsigned int amount,
		const string& kind);

	//�����տ�ݷ�����Ա
	bool assignExpress(Express* pExpress, const string& courierUsername);

	//���Ա���տ��
	void pickUpExpress(Express* pExpress);

	//�û�ǩ�տ��
	void signForExpress(Express* pExpress);

	//��������û���Ϣ
	const bool usersToString(ostringstream& oss, const int begin, const int end);

	//������п��Ա��Ϣ
	const bool couriersToString(ostringstream& oss, const int begin, const int end);

	//������п����Ϣ
	const bool expressesToString(ostringstream& oss, const int begin, const int end);

	//չʾ����δ������Ա�Ŀ����Ϣ
	const bool notAExpToString(ostringstream& oss, const int begin, const int end);

	inline unsigned int notAssignedListSize() { return notAssignedList.size(); }

	//���ݿ��Ա�û�����ѯ�û�
	Courier* findCourier(const string& username);

	//�����û�����ѯ�û�
	User* findUser(const string& username);

	//���ݿ�ݵ��Ų�ѯ���
	Express* findExpress(const unsigned int courierNumber);

	//���ݴ���ʱ���ѯ�����Ϣ
	const vector<const Express*> searchCreateTime(const time_t& begin, const time_t end);

	//�������ݵ�savePath
	void save(const string& savePath);

	//��loadPath�ж�ȡ����
	void load(const string& loadPath);
};
