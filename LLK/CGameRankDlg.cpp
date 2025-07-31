// CGameRankDlg.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "afxdialogex.h"
#include "CGameRankDlg.h"
#include "UserManagement.h"
#include "CGameControl.h"


// CGameRankDlg 对话框

IMPLEMENT_DYNAMIC(CGameRankDlg, CDialogEx)

CGameRankDlg::CGameRankDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RANK_DIALOG, pParent)
{

}

CGameRankDlg::~CGameRankDlg()
{
}

void CGameRankDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_RANK, m_TabRank);
}


BEGIN_MESSAGE_MAP(CGameRankDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_RANK, &CGameRankDlg::OnTcnSelchangeTabRank)
END_MESSAGE_MAP()


// CGameRankDlg 消息处理程序
void CGameRankDlg::InitWindow() {
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

void CGameRankDlg::SetTab() {
	CRect rect;
	GetClientRect(rect);
	int del = 10;
	rect.left += del;
	rect.top += del;
	rect.right -= del;
	rect.bottom -= del;
	rect.InflateRect(5, 5, 5, 5);
	m_TabRank.MoveWindow(rect);
}
//void CGameRankDlg::SetList() {
//	// 创建基本模式列表控件
//	m_ListBasic.Create(WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT, CRect(0, 0, 0, 0), &m_TabRank, IDC_RANK_BASIC);
//	m_ListBasic.InsertColumn(0, _T("排名"), LVCFMT_LEFT, 100);
//	m_ListBasic.InsertColumn(1, _T("积分"), LVCFMT_LEFT, 100);
//	m_ListBasic.InsertColumn(2, _T("用户"), LVCFMT_LEFT, 100);
//	m_ListBasic.InsertColumn(3, _T("时间"), LVCFMT_LEFT, 100);
//
//	// 创建休闲模式列表控件
//	m_ListRelax.Create(WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT, CRect(0, 0, 0, 0), &m_TabRank, IDC_RANK_RELAX);
//	m_ListRelax.InsertColumn(0, _T("排名1"), LVCFMT_LEFT, 100);
//	m_ListRelax.InsertColumn(1, _T("积分"), LVCFMT_LEFT, 100);
//	m_ListRelax.InsertColumn(2, _T("用户"), LVCFMT_LEFT, 100);
//	m_ListRelax.InsertColumn(3, _T("时间"), LVCFMT_LEFT, 100);
//
//	// 创建关卡模式列表控件
//	m_ListLevel.Create(WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT, CRect(0, 0, 0, 0), &m_TabRank, IDC_RANK_LEVEL);
//	m_ListLevel.InsertColumn(0, _T("排名2"), LVCFMT_LEFT, 100);
//	m_ListLevel.InsertColumn(1, _T("积分"), LVCFMT_LEFT, 100);
//	m_ListLevel.InsertColumn(2, _T("用户"), LVCFMT_LEFT, 100);
//	m_ListLevel.InsertColumn(3, _T("时间"), LVCFMT_LEFT, 100);
//}
void CGameRankDlg::SetListFont(CListBox* listBox, int fontSize) {
	listBox->SetFont(&listFont);
}

void CGameRankDlg::SetList() {
	// 创建列表控件对象
	CRect rect;
	m_TabRank.GetClientRect(rect);
	int nWidth = rect.Width();
	int nHeight = rect.Height();
	int nTop = rect.top + 30; // 调整列表框控件的位置
	int nLeft = rect.left + 10; // 调整列表框控件的位置
	int fontSize = 110;
	listFont.CreatePointFont(fontSize, _T("Times New Roman"));
	// 创建并设置基本模式列表框控件
	m_ListBasic.Create(WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_STANDARD,
		CRect(nLeft, nTop, nLeft + nWidth - 20, nTop + nHeight - 40),
		&m_TabRank, IDC_RANK_BASIC);
	BasicStr.Format(L"%2s   %2s   %2s   %10s   %12s", 
		L"排名", L"积分", L"用户", L"时间", L"剩余时间");
	m_ListBasic.SetFont(&listFont);

	// 创建并设置休闲模式列表框控件
	m_ListRelax.Create(WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_STANDARD,
		CRect(nLeft, nTop, nLeft + nWidth - 20, nTop + nHeight - 40),
		&m_TabRank, IDC_RANK_RELAX);
	RelaxStr.Format(L"%2s   %2s   %2s   %10s   %12s", 
		L"排名", L"积分", L"用户", L"时间", L"耗费时间");
	m_ListRelax.SetFont(&listFont);

	// 创建并设置关卡模式列表框控件
	m_ListLevel.Create(WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_STANDARD,
		CRect(nLeft, nTop, nLeft + nWidth - 20, nTop + nHeight - 40),
		&m_TabRank, IDC_RANK_LEVEL);
	m_ListLevel.SetFont(&listFont);
}

BOOL CGameRankDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitWindow();
	// TODO:  在此添加额外的初始化
	const char items[][15] = { "基本模式","休闲模式","关卡模式"};
	int i = 0;
	for (auto item : items) {
		m_TabRank.InsertItem(i++, (CString)item);
	}
	SetTab();
	SetList();
	// 隐藏所有列表控件
	m_ListBasic.ShowWindow(SW_SHOW);
	UpdateList(m_ListBasic, CGameControl::BASIC, BasicStr);
	m_ListRelax.ShowWindow(SW_HIDE);
	m_ListLevel.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CGameRankDlg::OnTcnSelchangeTabRank(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int nCurSel = m_TabRank.GetCurSel();
	m_ListBasic.ShowWindow(nCurSel == 0 ? SW_SHOW : SW_HIDE);
	m_ListRelax.ShowWindow(nCurSel == 1 ? SW_SHOW : SW_HIDE);
	m_ListLevel.ShowWindow(nCurSel == 2 ? SW_SHOW : SW_HIDE);
	switch (nCurSel) {
	case 0:
		UpdateList(m_ListBasic, CGameControl::BASIC, BasicStr); break;
	case 1:
		UpdateList(m_ListRelax, CGameControl::RELAX, RelaxStr); break;
	case 2:
		UpdateList(m_ListLevel, CGameControl::LEVEL, LevelStr); break;
	}
	*pResult = 0;
}

void CGameRankDlg::UpdateList(CListBox& m_ListBox, int mode,CString&Str) {
	int num = UserManagement::mg.recordNum;
	Record* record = UserManagement::mg.record;
	m_ListBox.ResetContent();				// 清空列表框
	int i = 0;
	m_ListBox.InsertString(i++,Str);
	while (record != NULL) {
		if (record->mode == mode) {
			str.Format(L"%4d    %04d   %6s   %16s   %s", i, record->score,
				(CString)record->username.c_str(), (CString)record->time.c_str(), 
				(CString)record->timer.c_str() );
			m_ListBox.InsertString(i++, str);
		}
		record = record->next;
	}
}
