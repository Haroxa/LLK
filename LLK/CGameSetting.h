#pragma once
#include "afxdialogex.h"


// CGameSetting 对话框

class CGameSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CGameSetting)

public:
	CGameSetting(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGameSetting();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_SettingTab;
	enum BG { WIDTH = 400, HEIGHT = 300 };


	void InitWindow();
	virtual BOOL OnInitDialog();
};
