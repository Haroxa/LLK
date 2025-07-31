#pragma once
#include "afxdialogex.h"


// CGameRankDlg 对话框

class CGameRankDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameRankDlg)

public:
	CGameRankDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGameRankDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RANK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_TabRank;
	CListBox m_ListBasic;
	CListBox m_ListRelax;
	CListBox m_ListLevel;

	CString BasicStr;
	CString RelaxStr;
	CString LevelStr;
	CString str;
	CFont listFont;

	//CListCtrl m_ListBasic;
	//CListCtrl m_ListRelax;
	//CListCtrl m_ListLevel;
	enum BG { WIDTH = 400, HEIGHT = 400 };

	void InitWindow();
	void SetTab();
	void SetListFont(CListBox* listBox, int fontSize);
	void SetList();
	void UpdateList(CListBox& m_ListBox, int mode, CString& Str);

	virtual BOOL OnInitDialog();

	afx_msg void OnTcnSelchangeTabRank(NMHDR* pNMHDR, LRESULT* pResult);
};
