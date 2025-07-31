#pragma once
#include "CGameLogic.h"
#include "UserManagement.h"

struct Timer {
	int hour, min, sec, del;
	void Increase() {
		if (++sec >= 60) {
			min++, sec = 0;
			if (min >= 60) hour++, min = 0;
		}
	}
	void Decrease() {
		if (--sec < 0) {
			min--, sec = 59;
			if (min < 0) hour--, min = 59;
		}
	}
	bool IsZero() {
		return hour == 0 && min == 0 && sec == 0;
	}
	void InitTime(int s) {
		sec = s % 60; s /= 60;
		min = s % 60; s /= 60;
		hour = s;
	}
	int GetSeconds() {
		return hour * 60 * 60 + min * 60 + sec;
	}
	int CalTimerFee() {
		double h = 3600, m = 60, s = 1, c = 0;
		// 一分钟以内
		if (hour == 0 && min == 0)
			s = 0.1;
		else
			c = -54;
		return h * hour + m * min + s * sec + c;
	}
	std::string ToString() {
		char str[10];
		sprintf_s(str, 10, "%02d:%02d:%02d", hour, min, sec);
		return str;
	}
};


class CGameControl
{
public:
	static int s_nRows;			// 游戏行数
	static int s_nCols;			// 游戏列数
	static int s_nPicNum;		// 图片数
	static UserManagement userMg;
	static User nowUser;
	enum MODE { BASIC, RELAX, LEVEL };
	int mode;
	Timer timer;
	int score;
	int** m_pGameMap;			// 游戏地图数组指针
	CGameLogic m_GameLogic;		// 游戏逻辑操作对象
	int vexNum;
	int cnt;
	int m_UseAuto;
	Vertex m_svSelFst;
	Vertex m_svSelSec;

	void SelectMode(int m);
	void StartGame();			
	int GetElement(int nRow, int nCol);

	void SetFirstPoint(int nRow, int nCol);
	void SetSecPoint(int nRow, int nCol);
	bool Link(Vertex* avPath, int& nVexnum);
	void ClearPoint();

	void TimerUpdateScore(int& score, int mode);
	bool IsWin(void);
	void ResetMap();
	bool ReadTempData();
	void StoreTempData();
	void GetTip(Vertex* avPath, int& nVexnum);
	
	void Login(User& user);
	void Logout();
};