
// LLKDlg.h: 头文件
//

#pragma once


// CLLKDlg 对话框
class CLLKDlg : public CDialogEx
{
// 构造
public:
	CLLKDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LLK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CDC m_dcMem;
	enum BG{ WIDTH = 800, HEIGHT = 600 };
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	// 新增函数
	void InitBackground();
public:
	int ShowMessage(const char* message, const char* title, int mode = MB_OK | MB_ICONINFORMATION);

	afx_msg void OnBnClickedBtnBasic();
	afx_msg void OnClickedBtnSetting();
	afx_msg void OnBnClickedBtnLogin();
	CStatic UserStatusStr;
	afx_msg void OnBnClickedBtnRelax();
	afx_msg void OnBnClickedBtnRank();
	afx_msg void OnBnClickedBtnHelp();
};
