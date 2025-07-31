#pragma once
#include "afxdialogex.h"
#include "CGameControl.h"
#include "iostream"

struct IDC{
	UINT START, STOP, TIP, RESET, TIMER, SCORE, LIST, HELP, AUTO, AUTONUM;
};

// CGameDlg 对话框

class CGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDlg)

public:
	CGameDlg(CWnd* pParent = nullptr,UINT ID=IDD_GAME_DIALOG);   // 标准构造函数
	virtual ~CGameDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DIALOG };
#endif

	static CGameControl m_GameC;					// 游戏控制类

protected:
	// 内存 元素 掩码 DC
	CDC m_dcMem, m_dcBG, m_dcElement, m_dcMask;
	CPoint m_ptGameTop;						// 游戏区起始点
	CSize m_sizeElem;						// 元素图片大小
	CRect m_rtGameRect;						// 游戏区域大小
	enum BG { WIDTH = 800, HEIGHT = 600 };
	static IDC MIDC;
	bool m_bFirstPoint;						// 是否选择了一个点
	bool m_bPlaying;						// 游戏是否进行中
	Vertex avPath[PATH_LEN] = { 0 };		// 连接路径
	int nVexnum;							// 路径点数
	bool m_FirstOpen;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	// 新增函数
	
	void InitBackground();
	void InitElement();
	void UpdateWindow();
	void UpdateMap();
	void UpdateList();
	void UpdateTimer();
	void UpdateScore();
	void DrawTipFrame(int nRow, int nCol);
	void DrawTipLine(Vertex avPath[], int nVexnum);
	void ClearPoint(Vertex v1);
	

public:
	CFont listFont;

	int ShowMessage(const char* message, const char* title, int mode = MB_OK | MB_ICONINFORMATION);
	void SetListFont(CListBox* listBox, int fontSize);
	void ShowGameOver();

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnTip();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_TimerStr;
	CStatic m_ScoreStr;
	CListBox m_ListStr;

	afx_msg void OnBnClickedBtnHelpBasic();
	afx_msg void OnBnClickedBtnAutoBasic();
	CEdit m_AutoNumStr;
};
