#include "pch.h"
#include "UserManagement.h"
#include "iomanip"
#include "fstream"
#include "regex"

std::istream& operator>>(std::istream& in, Record& record) {
	return in >> record.time >> record.score >> record.mode >> record.username >> record.timer;
}
std::ostream& operator<<(std::ostream& out, Record& record) {
	return out << std::setw(20) << record.time << std::setw(5) << record.score 
		<< std::setw(4) << record.mode << std::setw(8) << record.username << std::setw(10) << record.timer;
}
std::istream& operator>>(std::istream& in, User& user) {
	in >> user.username >> user.password;// >> user.avatar;
	//in >> user.status;
	in >> user.recordNum;
	if (user.recordNum)
		user.record = new Record{};
	Record* ptr = user.record, * temp = NULL;
	for (int i = 0; i < user.recordNum; i++) {
		if (i) ptr = new Record{};
		in >> (*ptr);
		ptr->prev = temp;
		if(temp!=NULL) temp->next = ptr;
		temp = ptr;
		ptr = ptr->next;
		if (UserManagement::mg.status == User::STATUS::MANAGER)
			UserManagement::mg.AddRecord(temp);
	}
	return in;
}
std::ostream& operator<<(std::ostream& out, User& user) {
	out << std::setw(15) << user.username << std::setw(15) << user.password << "\n";
	//out << user.avatar << "\n";
	//out << user.status << "\n";
	out << user.recordNum << "\n";
	Record* ptr = user.record, *temp;
	for (int i = 0; i < user.recordNum; i++) {
		out << (*ptr) << "\n";
		temp = ptr;
		ptr = ptr->next;
	}
	return out;
}
bool User::Check(std::string& source,const std::string& pattern) {
	std::regex regex(pattern);
	return std::regex_search(source, regex);
}

void User::AddRecord(Record* r) {
	if (status != STATUS::MANAGER)
		r->username = username;
	if (username!="" && status != STATUS::MANAGER)
		UserManagement::mg.AddRecord(r);
	r = new Record{ *r };
	if (record == NULL)
		record = r;
	else {
		Record* temp = record;
		while (temp->next != NULL && r->score < temp->score)
			temp = temp->next;
		// 处理最后一条记录的比较
		if (temp->next == NULL && r->score < temp->score) {
			temp->next = r;
			r->prev = temp;
		}
		else {
			if (temp->prev != NULL)	// 处理第一条记录
				temp->prev->next = r;
			else record = r;		// 更新头节点
			r->prev = temp->prev;
			r->next = temp;
			temp->prev = r;
		}
	}
	recordNum++;
}

UserManagement::UserManagement() {
	InitUsers();
}

User UserManagement::Query(const User& u) {
	// 不存在
	if (users.count(u.username) == 0)
		return {};
	User user = users[u.username];
	// 只根据用户名查询 或 根据用户密码查询
	if (u.password.empty() || u.password == user.password)
		return user;
	return {};
}
bool UserManagement::Add(const User&u) {
	// 已存在
	if (users.count(u.username) != 0)
		return false;
	users[u.username] = u;
	StoreUsers();
	return true;
}
bool UserManagement::Delete(const User& u) {
	// 不存在
	if (users.count(u.username) == 0)
		return false;
	users.erase(u.username);
	StoreUsers();
	return true;
}

bool UserManagement::Update(const User& u) {
	// 不存在
	if (users.count(u.username) == 0)
		return false;
	users[u.username] = u;
	StoreUsers();
	return true;
}

void UserManagement::InitUsers() {
	std::ifstream in(filePath);
	int num;
	in >> num;
	User user{};
	for (int i = 0; i < num; i++) {
		in >> user;	
		users[user.username] = user;
		
	}
	in.close();
}
void UserManagement::StoreUsers() {
	std::ofstream out(filePath);
	int num = users.size();
	out << num << "\n";
	User user;
	for (auto item:users) {
		user = item.second;
		out << user << "\n";
	}
	out.close();
}
