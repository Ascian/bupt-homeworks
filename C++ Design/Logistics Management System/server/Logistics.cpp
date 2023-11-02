#include "Logistics.h"

bool Logistics::createUserAccount(const string& username, const string& password, const string& name,
	const unsigned _int64 phone, const string& address)
{
	if (users.size() >= users.max_size()) {
		throw(char)ELEMENT_EXCEED_LIMIT;
		return false;
	}
	if (users.contains(username)) {
		throw (char)ACCOUNT_ALREADY_EXIST;
		return false;
	}
	User* newAccount = new User(username, password,name,phone,address);
	users.insert({ username , newAccount });
	return true;
}

bool Logistics::createCourierAccount(const string& username, const string& password, const string& name, const unsigned _int64 phone)
{
	if (couriers.size() >= couriers.max_size()) {
		throw (char)ELEMENT_EXCEED_LIMIT;
		return false;
	}
	if (couriers.contains(username)) {
		throw (char)ACCOUNT_ALREADY_EXIST;
		return false;
	}
	Courier* newAccount = new Courier(username, password, name, phone);
	couriers.insert({ username , newAccount });
	return true;
}

bool Logistics::loginManagerAccount(const string& username, const string& password)
{
	if (manager.getUsername() != username) {
		throw (char)ELEMENT_NOT_FOUND;
		return false;
	}
	if (manager.getPassword() != password) {
		throw (char)PASSWORD_ERROR;
		return false;
	}
	return true;
}

User* Logistics::loginUserAccount(const string& username, const string& password) {
	if (!users.contains(username)) {
		throw (char)ELEMENT_NOT_FOUND;
		return NULL;
	}
	auto temp = users.find(username);
	if (temp->second->getPassword() != password) {
		throw(char)PASSWORD_ERROR;
		return NULL;
	}
	return temp->second;
}

Courier* Logistics::loginCourierAccount(const string& username, const string& password)
{
	if (!couriers.contains(username)) {
		throw (char)ELEMENT_NOT_FOUND;
		return NULL;
	}
	auto temp = couriers.find(username);
	if (temp->second->getPassword() != password) {
		throw (char)PASSWORD_ERROR;
		return NULL;
	}
	return temp->second;
}

bool Logistics::deleteCourierAccount(const string& username)
{
	if (!couriers.contains(username)) {
		throw (char)ELEMENT_NOT_FOUND;
		return false;
	}
	couriers.erase(couriers.find(username));
	return true;
}

void createExpress(Express*& pExpress, const string& senderUsername, const string& receiverUsername,
	const time_t creatTime, const string& condition, const string& description, const unsigned int courierNumber,
	const unsigned int amount, const string& kind) {
	delete(pExpress);

	if (kind == Express::subKind().at(0))
		pExpress = new NormalExpress(senderUsername, receiverUsername, creatTime, condition, description, courierNumber, amount);
	else if (kind == Express::subKind().at(1))
		pExpress = new Fragile(senderUsername, receiverUsername, creatTime, condition, description, courierNumber, amount);
	else if (kind == Express::subKind().at(2))
		pExpress = new Book(senderUsername, receiverUsername, creatTime, condition, description, courierNumber, amount);
	else
		pExpress = NULL;
}

unsigned int Logistics::createLogistics(const string& senderUsername, const string& receiverUsername, 
	const string& condition, const string& description, const unsigned int amount, const string& kind)
{
	if (senderUsername == receiverUsername) {
		throw (char)RECEIVER_EQUAL_SENDER;
		return UINT_MAX;
	}

	auto temp = users.find(senderUsername);
	if (temp == users.end()) {
		throw (char)SENDER_NOT_EXIST;
		return UINT_MAX;
	}

	User* sender = temp->second;
	temp = users.find(receiverUsername);
	if (temp == users.end()) {
		throw (char)RECEIVER_NOT_EXIST;
		return UINT_MAX;
	}
	User* receiver = temp->second;

	//创建快递单号
	if (expresses.size() >= expresses.max_size()) {
		throw (char)ELEMENT_EXCEED_LIMIT;
		return UINT_MAX;
	}
	unsigned int courierNumber = expresses.size();

	Express* pExpress = NULL;
	createExpress(pExpress, sender->getUsername(), receiver->getUsername(), time(0), 
		condition, description, courierNumber, amount, kind);
	if (!pExpress) {
		throw(char)(EXPRESS_UNKNOWN_KIND);
		return UINT_MAX;
	}

	if (sender->getBalance() < pExpress->getPrice()) {
		//余额不足
		throw (char)BALANCE_NOT_ENOUGH;
		return UINT_MAX;
	}

	expresses.insert({ courierNumber,pExpress });
	createTimeList.insert({ pExpress->getCreatTime(), pExpress});
	notAssignedList.insert(pExpress);

	//发送方转账给管理员
	sender->subBalance(pExpress->getPrice());
	manager.addBalance(pExpress->getPrice());

	//修改发送方快递信息
	sender->addNotSendedList(pExpress);//加入未发出快递列表
	sender->addReceiverList(pExpress);//加入接收人查找表

	//修改接收方快递信息
	receiver->addNotPickedList(pExpress);//加入为揽收快递列表
	receiver->addSenderList(pExpress);//加入发送人查找表

	return courierNumber;
}

bool Logistics::assignExpress(Express* pExpress, const string& courierUsername){
	if (pExpress->getSendTime() != NOT_ASSIGNED) {
		throw (char)EXPRESS_ALREADY_ASSIGN;
		return false;
	}
	
	auto temp = couriers.find(courierUsername);
	if (temp == couriers.end()) {
		throw (char)ELEMENT_NOT_FOUND;
		return false;
	}
	Courier* pCourier = temp->second;

	notAssignedList.erase(pExpress);//删除未分配快递
	pExpress->setCourier(courierUsername);
	pExpress->setSendTime(NOT_SENDED);

	//修改快递员快递信息
	pCourier->addNotPickedList(pExpress);
	pCourier->addSenderList(pExpress);
	pCourier->addReceiverList(pExpress);
	return true;
}

void Logistics::pickUpExpress(Express* pExpress)
{
	if (pExpress->getSendTime() != NOT_SENDED) {
		throw(char)(EXPRESS_ALREADY_PICKUP);
		return;
	}
	pExpress->setSendTime(time(0));

	//修改快递员快递信息
	Courier* pCourier = couriers.find(pExpress->getCourier())->second;
	pCourier->deleteNotPickedList(pExpress);//删除待揽收快递
	pCourier->addNotReceivedList(pExpress);//加入待签收快递列表
	pCourier->addPickTimeList(pExpress);//加入已揽收快递列表

	//管理员转账给快递员
	pCourier->addBalance((double)pExpress->getPrice() / 2);
	manager.subBalance((double)pExpress->getPrice() / 2);

	//修改发送方快递信息
	User* sender = users.find(pExpress->getSender())->second;
	sender->deleteNotSendedList(pExpress);//删除未发出快递
	sender->addSendList(pExpress);//加入已发出快递列表

	//修改接收方快递信息
	User* receiver = users.find(pExpress->getReceiver())->second;
	receiver->deleteNotPickedList(pExpress);//删除未揽收快递
	receiver->addNotReceivedList(pExpress);//加入待签收快递列表
}

void Logistics::signForExpress(Express* pExpress)
{
	if (pExpress->getSendTime() == NOT_ASSIGNED || pExpress->getSendTime() == NOT_SENDED) {
		throw(char)(EXPRESS_FORBIT_SIGNFOR);
		return;
	}
	if (pExpress->getReceiveTime() != NOT_RECEIVED) {
		throw(char)(EXPRESS_ALREADY_SIGNFOR);
		return;
	}

	pExpress->setReceiveTime(time(0));

	//修改快递员快递信息
	Courier* pCourier = couriers.find(pExpress->getCourier())->second;
	pCourier->deleteNotReceivedList(pExpress);//删除待签收快递
	pCourier->addReceivedList(pExpress);//加入已签收快递列表

	//修改接收方快递信息
	User* receiver = users.find(pExpress->getReceiver())->second;
	receiver->deleteNotReceivedList(pExpress);//删除待签收快递
	receiver->addReceiveList(pExpress);//加入已签收快递列表
}

const bool Logistics::usersToString(ostringstream& oss, const int begin, const int end){
	if (users.size() == 0) {
		throw(char)(CONTAINER_EMPTY);
		return false;
	}
	unsigned int i = 1;
	auto iter = users.begin();
	for (; i < begin && iter!= users.end(); i++, iter++);
	for (; i <= end && iter!= users.end(); i++, iter++) {
		oss << "[" << i << "]" << endl;
		oss << *(iter->second) << endl;	
	}
	if (iter != users.end()) {
		return true;
	}
	return false;
}

const bool Logistics::couriersToString(ostringstream& oss, const int begin, const int end)
{
	if (couriers.size() == 0) {
		throw(char)(CONTAINER_EMPTY);
		return false;
	}
	unsigned int i = 1;
	auto iter = couriers.begin();
	for (; i < begin && iter != couriers.end(); i++, iter++);
	for (; i <= end && iter != couriers.end(); i++, iter++) {
		oss << "[" << i << "]" << endl;
		oss << *(iter->second) << endl;
	}
	if (iter != couriers.end()) {
		return true;
	}
	return false;
}

const bool Logistics::expressesToString(ostringstream& oss, const int begin, const int end){
	if (expresses.size() == 0) {
		throw(char)(CONTAINER_EMPTY);
		return false;
	}
	unsigned int i = 1;
	auto iter = expresses.begin();
	for (; i < begin && iter != expresses.end(); i++, iter++);
	for (; i <= end && iter != expresses.end(); i++, iter++) {
		oss << "[" << i << "]" << endl;
		oss << *(iter->second) << endl;
	}
	if (iter != expresses.end()) {
		return true;
	}
	return false;
}

const bool Logistics::notAExpToString(ostringstream& oss, const int begin, const int end)
{
	if (notAssignedList.size() == 0) {
		throw(char)(CONTAINER_EMPTY);
		return false;
	}
	unsigned int i = 1;
	auto iter = notAssignedList.begin();
	for (; i < begin && iter != notAssignedList.end(); i++, iter++);
	for (; i <= end && iter != notAssignedList.end(); i++, iter++) {
		oss << "[" << i << "]" << endl;
		oss << **iter << endl;
	}
	if (iter != notAssignedList.end()) {
		return true;
	}
	return false;
}

Courier* Logistics::findCourier(const string& username)
{
	auto temp = couriers.find(username);
	if (temp == couriers.end()) {
		throw(char)(ELEMENT_NOT_FOUND);
		return NULL;
	}
	return temp->second;
}

User* Logistics::findUser(const string& username)
{
	auto temp = users.find(username);
	if (temp == users.end()) {
		throw(char)(ELEMENT_NOT_FOUND);
		return NULL;
	}
	return temp->second;
}

Express* Logistics::findExpress(const unsigned int courierNumber)
{
	auto temp = expresses.find(courierNumber);
	if (temp == expresses.end()) {
		throw(char)(ELEMENT_NOT_FOUND);
		return NULL;
	}
	return temp->second;
}

const vector<const Express*> Logistics::searchCreateTime(const time_t& begin, const time_t end)
{
	vector<const Express*> result;
	for (auto temp = createTimeList.lower_bound(begin); temp != createTimeList.end() && temp->first <= end; temp++) {
		result.push_back(temp->second);
	}
	if (result.size() == 0)
		throw(char)(ELEMENT_NOT_FOUND);
	return result;
}

void Logistics::save(const string& savePath){
	ofstream managerFile(savePath + "/Manager", ios::out | ios::trunc);
	managerFile << manager;
	managerFile.close();

	ofstream courierFile(savePath + "/Couriers", ios::out | ios::trunc);
	courierFile << couriers.size() << endl << endl;
	for (auto& courier : couriers) {
		courierFile << *courier.second << endl;
	}
	courierFile.close();

	ofstream userFile(savePath+"/Users", ios::out | ios::trunc);
	userFile << users.size() << endl << endl;
	for (auto& user : users) {
		userFile << *user.second << endl;
	}
	userFile.close();

	ofstream expFile(savePath + "/Expresses", ios::out | ios::trunc);
	expFile << expresses.size() << endl << endl;
	for (auto& express : expresses) {
		expFile << *express.second << endl;
	}
	expFile.close();
}



void loadExpress(istream& Istr, Express*& pExpress) {
	delete(pExpress);

	string kind;
	Istr.ignore(strlen("Kind: "));
	Istr >> kind;
	if (kind == Express::subKind().at(0))
		pExpress = new NormalExpress();
	else if (kind == Express::subKind().at(1))
		pExpress = new Fragile();
	else if (kind == Express::subKind().at(2))
		pExpress = new Book();
	Istr >> *pExpress;
}

void Logistics::load(const string& loadPath){
	ifstream managerFile(loadPath + "/Manager", ios::in);
	if (!managerFile.is_open()) {
		throw("File \"/Manager\" does not exist");
		return;
	}
	managerFile >> manager;
	managerFile.close();

	ifstream courierFile(loadPath + "/Couriers", ios::in);
	if (!courierFile.is_open()) {
		throw("File \"/Couriers\" does not exist");
		return;
	}
	unsigned int num = 0;
	courierFile >> num;
	courierFile.ignore(1);
	for (unsigned int i = 0; i < num; i++) {
		Courier* pCourier = new Courier();
		courierFile >> *pCourier;
		couriers.insert({ pCourier->getUsername(),pCourier });
		courierFile.ignore(1);
	}
	courierFile.close();

	ifstream userFile(loadPath + "/Users", ios::in);
	if (!userFile.is_open()) {
		throw("File \"/Users\" does not exist");
		return;
	}
	userFile >> num;
	userFile.ignore(1);
	for (unsigned int i = 0; i < num; i++) {
		User* pUser = new User();
		userFile >> *pUser;
		users.insert({ pUser->getUsername(),pUser});
		userFile.ignore(1);
	}
	userFile.close();

	ifstream expFile(loadPath + "/Expresses", ios::in);
	if (!expFile.is_open()) {
		throw("File \"/Expresses\" does not exist");
		return;
	}
	expFile >> num;
	expFile.ignore(1);
	for (unsigned int i = 0; i < num; i++) {
		Express* pExpress = NULL;
		loadExpress(expFile, pExpress);

		//加入管理员各类查找表
		expresses.insert({ pExpress->getCourierNumber(),pExpress});
		createTimeList.insert({pExpress->getCreatTime(), pExpress});
		if (pExpress->getSendTime() == NOT_ASSIGNED)
			notAssignedList.insert(pExpress);
		else {
			//加入快递员各类查找表
			Courier* pCourier = couriers.find(pExpress->getCourier())->second;
			if (pExpress->getSendTime() == NOT_SENDED) {
				pCourier->addNotPickedList(pExpress);
			}
			else {
				pCourier->addPickTimeList(pExpress);
				if (pExpress->getReceiveTime() == NOT_RECEIVED)
					pCourier->addNotReceivedList(pExpress);
				else
					pCourier->addReceivedList(pExpress);
			}
			pCourier->addSenderList(pExpress);
			pCourier->addReceiverList(pExpress);
		}
		
		//加入发送方各类查找表
		User* pUser = users.find(pExpress->getSender())->second;
		if (pExpress->getSendTime() == NOT_ASSIGNED || pExpress->getSendTime() == NOT_SENDED) {
			pUser->addNotSendedList(pExpress);
		}
		else
			pUser->addSendList(pExpress);
		pUser->addReceiverList(pExpress);


		//加入接收方各类查找表
		pUser = users.find(pExpress->getReceiver())->second;
		if (pExpress->getSendTime() == NOT_ASSIGNED || pExpress->getSendTime() == NOT_SENDED)
			pUser->addNotPickedList(pExpress);
		else if (pExpress->getReceiveTime() == NOT_RECEIVED)
			pUser->addNotReceivedList(pExpress);
		else
			pUser->addReceiveList(pExpress);
		pUser->addSenderList(pExpress);
		expFile.ignore(1);
	}
	expFile.close();
}

