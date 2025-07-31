#include "pch.h"
#include "CGameControl.h"
#include "UserManagement.h"

void CGameControl::TimerUpdateScore(int& score, int mode) {
	int div = 10 + m_UseAuto;
	switch (mode) {
	case CGameControl::MODE::BASIC:
		timer.del = 10.0 * timer.GetSeconds() / div;
		break;
	case CGameControl::MODE::RELAX:
		timer.del = -timer.CalTimerFee();
		break;
	default:
		break;
	}
	score += timer.del;
}

int CGameControl::s_nRows = NR;
int CGameControl::s_nCols = NC;
int CGameControl::s_nPicNum = 10;
User CGameControl::nowUser = {};
User UserManagement::mg = { "manager","123",0,User::STATUS::MANAGER };
UserManagement CGameControl::userMg = {};

char tempMapPath[30] = "./data/tempMap/00.txt";

void CGameControl::SelectMode(int m) {
	mode = m;
	sprintf_s(tempMapPath, sizeof(char) * 30, 
		"./data/tempMap/%s_%d.txt", nowUser.username.c_str(), mode);
	if (mode == BASIC) {
		timer = { 0,10,0 };
	}
	else if (mode == RELAX) {
		timer = {};
	}
}

void CGameControl::StartGame() {
	m_pGameMap = m_GameLogic.InitMap();
	vexNum = NR * NC;
	score = 0;
	if (mode == BASIC) {
		timer = { 0,10,0 };
	}
	else if (mode == RELAX) {
		timer = {};
	}
	m_UseAuto = 0;
}

int CGameControl::GetElement(int nRow, int nCol) {
	if(nRow >= s_nRows || nCol >= s_nCols)
		throw new CGameException(_T("地图下标越界！\n"));
	return m_pGameMap[nRow][nCol];
}

void CGameControl::SetFirstPoint(int nRow, int nCol) {
	m_svSelFst = { nRow, nCol, BLANK };
}

void CGameControl::SetSecPoint(int nRow, int nCol) {
	m_svSelSec = { nRow, nCol, BLANK };
}

bool CGameControl::Link(Vertex* avPath, int& nVexnum) {
	// 判断是否同一张图片
	if (m_svSelFst.row == m_svSelSec.row && m_svSelFst.col == m_svSelSec.col)
		return false;
	// 判断图片是否相同
	if (m_pGameMap[m_svSelFst.row][m_svSelFst.col] != 
		m_pGameMap[m_svSelSec.row][m_svSelSec.col])
		return false;
	// 判断是否连通
	if( m_GameLogic.IsLinkBFS(m_pGameMap, m_svSelFst, m_svSelSec) )
	{
		// 消子
		m_GameLogic.Clear(m_pGameMap, m_svSelFst, m_svSelSec);
		vexNum -= 2;
		// 返回路径顶点
		nVexnum = m_GameLogic.GetVexPath(avPath);
		m_GameLogic.UpdateScore(mode,score);
		StoreTempData();
		return true;
	}
	return false;
}

void CGameControl::ClearPoint() {
	m_svSelFst = m_svSelSec = {};
	m_GameLogic.ClearStack();
}

bool CGameControl::IsWin() {
	if (vexNum == 0 ) {
		TimerUpdateScore(score, mode);
		Record record = { score, mode };
		record.time = getNowTime();
		record.timer = timer.ToString();
		nowUser.AddRecord(&record);
		userMg.Update(nowUser);
		remove(tempMapPath);
		return true;
	}
	return false;
}

void CGameControl::ResetMap() {
	m_GameLogic.ResetMap(m_pGameMap);
}


bool CGameControl::ReadTempData() {
	std::ifstream in(tempMapPath);
	// 分配空间
	m_pGameMap = m_GameLogic.InitMap(false);
	for (int i = 0; i < s_nRows; i++) {
		for (int j = 0; j < s_nCols; j++)
			in >> m_pGameMap[i][j];
	}
	int seconds;
	in >> score >> seconds >> vexNum;
	timer.InitTime(seconds);
	return in.good();
}

void CGameControl::StoreTempData() {
	if (m_pGameMap == NULL && !ReadTempData())
		return;
	std::ofstream out(tempMapPath);
	for (int i = 0; i < s_nRows; i++) {
		for (int j = 0; j < s_nCols; j++)
			out << m_pGameMap[i][j] << " \n"[j == s_nCols - 1];
	}
	out << score << " " << timer.GetSeconds() << " " << vexNum << "\n";
}


void CGameControl::GetTip(Vertex* avPath, int& nVexnum) {
	m_GameLogic.GetTip(m_pGameMap);
	nVexnum = m_GameLogic.GetVexPath(avPath);
}

void CGameControl::Login(User&user) {
	Record* r = nowUser.record;
	while (r != NULL) {
		user.AddRecord(r);
		r = r->next;
	}
	if (nowUser.recordNum) userMg.Update(user);
	nowUser = user;
	nowUser.status = User::STATUS::LOGIN;
	remove(tempMapPath);
	sprintf_s(tempMapPath, sizeof(char) * 30, 
		"./data/tempMap/%s_%d.txt", nowUser.username.c_str(), mode);
	if(m_pGameMap!=NULL) StoreTempData();
}

void CGameControl::Logout() {
	nowUser = {};
}