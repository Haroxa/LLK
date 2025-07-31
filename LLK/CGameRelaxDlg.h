#pragma once
#include "afxdialogex.h"
#include "CGameDlg.h"

// CGameRelaxDlg 对话框

class CGameRelaxDlg : public CGameDlg
{
	DECLARE_DYNAMIC(CGameRelaxDlg)

public:
	CGameRelaxDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGameRelaxDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RELAX_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	afx_msg void OnBnClickedBtnStartRelax();
	afx_msg void OnBnClickedBtnStopRelax();
	afx_msg void OnBnClickedBtnTipRelax();
	afx_msg void OnBnClickedBtnResetRelax();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnHelpRelax();
	afx_msg void OnBnClickedBtnAutoRelax();
};
