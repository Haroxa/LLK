// CGameSetting.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "afxdialogex.h"
#include "CGameSetting.h"
#include "CGameDlg.h"
#include "iostream"


// CGameSetting 对话框

IMPLEMENT_DYNAMIC(CGameSetting, CDialogEx)

CGameSetting::CGameSetting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTING_DIALOG, pParent)
{

}

CGameSetting::~CGameSetting()
{
}

void CGameSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETTING_TAB, m_SettingTab);
}


BEGIN_MESSAGE_MAP(CGameSetting, CDialogEx)
END_MESSAGE_MAP()


// CGameSetting 消息处理程序


void CGameSetting::InitWindow() {
	// 调整窗口大小
	CRect rtWin;
	CRect rtClient;
	this->GetWindowRect(rtWin);		// 获得窗口大小
	this->GetClientRect(rtClient);	// 获得客户区大小
	// 标题栏和外边框的大小
	int nSpanWidth = rtWin.Width() - rtClient.Width();
	int nSpanHeight = rtWin.Height() - rtClient.Height();
	// 设置窗口大小
	MoveWindow(0, 0, BG::WIDTH + nSpanWidth, BG::HEIGHT + nSpanHeight);
	// 窗口居中
	CenterWindow();
}

BOOL CGameSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitWindow();
	// TODO:  在此添加额外的初始化
	const char items[][15] = {"个人信息","历史记录","常规设置","其它设置"};
	int i = 0;
	for (auto item: items) {
		m_SettingTab.InsertItem(i++, (CString)item);

	}
	//m_SettingTab.SetItemSize(CSize{ BG::WIDTH,BG::HEIGHT });
	
	CRect rect;
	GetClientRect(rect);
	int del = 10;
	rect.left += del;
	rect.top += del;
	rect.right -= del;
	rect.bottom -= del;
	rect.InflateRect(5, 5, 5, 5);
	m_SettingTab.MoveWindow(rect);
	//m_SettingTab.GetWindowRect(rect);
	//this->GetWindowRect(rect);
	//m_SettingTab.AdjustRect(TRUE, rect);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
