#pragma once
#include "afxdialogex.h"
#include "iostream"

// CGameUser 对话框

class CGameUser : public CDialogEx
{
	DECLARE_DYNAMIC(CGameUser)

public:
	CGameUser(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGameUser();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	enum BG { WIDTH = 310, HEIGHT = 200 };
	bool m_bUserStatus;
	std::string username, password;

	int ShowMessage(const char* message, const char* title, int mode = MB_OK | MB_ICONINFORMATION);
	void InitWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedLoginOk();
	CEdit UsernameStr;
	CEdit PasswordStr;
};
