#pragma once
#include "iostream"
#include "map"

static std::string getNowTime() {
	time_t t;
	tm tm;
	time(&t);							// 返回 1970年1月1日 至今所经历的时间（以秒为单位）
	localtime_s(&tm,&t);					// 匹配 本地 时间格式
	char tp[30];
	strftime(tp, 30, "%y.%m.%d-%H:%M", &tm);// 按自定义形式输出
	return std::string(tp);
}

struct Record {
	int score, mode;
	std::string time, username, timer;
	Record* next, *prev;
	friend std::istream& operator>>(std::istream& in, Record& record);
	friend std::ostream& operator<<(std::ostream& out, Record& record);
};

struct User {
	enum STATUS {LOGOUT, LOGIN, MANAGER};
	std::string username, password;// , avatar;
	int recordNum, status;
	Record* record;
	friend std::istream& operator>>(std::istream& in, User& user);
	friend std::ostream& operator<<(std::ostream& out, User& user);
	static bool Check(std::string& source, const std::string& pattern);
	void AddRecord(Record* r);
};

class UserManagement
{
public:
	const char* filePath = "./data/users.txt";
	std::map<std::string, User> users;
	static User mg;
	UserManagement();
	User Query(const User& u);
	bool Add(const User& u);
	bool Delete(const User& u);
	bool Update(const User& u);
	void InitUsers();
	void StoreUsers();
};
