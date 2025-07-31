// CGameRelaxDlg.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "afxdialogex.h"
#include "CGameRelaxDlg.h"

// CGameRelaxDlg 对话框

IMPLEMENT_DYNAMIC(CGameRelaxDlg, CDialogEx)

CGameRelaxDlg::CGameRelaxDlg(CWnd* pParent /*=nullptr*/)
	: CGameDlg(pParent, IDD_RELAX_DIALOG)
	//: CDialogEx(IDD_RELAX_DIALOG, pParent)
{
	//MIDC = {
	//	IDC_BTN_START_RELAX, IDC_BTN_STOP_RELAX, IDC_BTN_TIP_RELAX, IDC_BTN_RESET_RELAX,
	//	IDC_TIMER_RELAX, IDC_SCORE_RELAX, IDC_LIST_RELAX
	//};
}

CGameRelaxDlg::~CGameRelaxDlg()
{
}

void CGameRelaxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, MIDC.TIMER, m_TimerStr);
	DDX_Control(pDX, MIDC.SCORE, m_ScoreStr);
	DDX_Control(pDX, MIDC.LIST, m_ListStr);
	DDX_Control(pDX, MIDC.AUTONUM, m_AutoNumStr);
}


BEGIN_MESSAGE_MAP(CGameRelaxDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(MIDC.START, &CGameRelaxDlg::OnBnClickedBtnStartRelax)
	ON_BN_CLICKED(MIDC.STOP, &CGameRelaxDlg::OnBnClickedBtnStopRelax)
	ON_BN_CLICKED(MIDC.TIP, &CGameRelaxDlg::OnBnClickedBtnTipRelax)
	ON_BN_CLICKED(MIDC.RESET, &CGameRelaxDlg::OnBnClickedBtnResetRelax)
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(MIDC.HELP, &CGameRelaxDlg::OnBnClickedBtnHelpRelax)
	ON_BN_CLICKED(MIDC.AUTO, &CGameRelaxDlg::OnBnClickedBtnAutoRelax)
END_MESSAGE_MAP()


// CGameRelaxDlg 消息处理程序


BOOL CGameRelaxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ptGameTop = CPoint(15, 45);
	m_bFirstPoint = true;
	m_bPlaying = false;

	m_GameC.SelectMode(m_GameC.RELAX);

	GetDlgItem(MIDC.START)->EnableWindow(!m_bPlaying);
	GetDlgItem(MIDC.STOP)->EnableWindow(m_bPlaying);
	GetDlgItem(MIDC.TIP)->EnableWindow(m_bPlaying);
	GetDlgItem(MIDC.RESET)->EnableWindow(m_bPlaying);

	InitBackground();
	InitElement();
	UpdateList();
	SetListFont(&m_ListStr, 61);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

bool IsNeedRelaxHelp = true;
void CGameRelaxDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CGameDlg::OnPaint()
	dc.BitBlt(0, 0, BG::WIDTH, BG::HEIGHT, &m_dcMem, 0, 0, SRCCOPY);
	if (IsNeedRelaxHelp) {
		OnBnClickedBtnHelpRelax();
		IsNeedRelaxHelp = false;
	}
}

void CGameRelaxDlg::OnBnClickedBtnStartRelax()
{
	// TODO: 在此添加控件通知处理程序代码
	CGameDlg::OnBnClickedBtnStart();
}


void CGameRelaxDlg::OnBnClickedBtnStopRelax()
{
	// TODO: 在此添加控件通知处理程序代码
	CGameDlg::OnBnClickedBtnStop();
}


void CGameRelaxDlg::OnBnClickedBtnTipRelax()
{
	// TODO: 在此添加控件通知处理程序代码
	CGameDlg::OnBnClickedBtnTip();
}


void CGameRelaxDlg::OnBnClickedBtnResetRelax()
{
	// TODO: 在此添加控件通知处理程序代码
	CGameDlg::OnBnClickedBtnReset();
}


void CGameRelaxDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (!m_GameC.timer.IsZero() && m_GameC.score < m_GameC.timer.CalTimerFee() ) {
		KillTimer(MIDC.TIMER);
		ShowMessage("游戏结束，当前积分无法支付时间消耗", "提示");
		GetDlgItem(MIDC.START)->EnableWindow(true);
		GetDlgItem(MIDC.START)->SetWindowText(_T("重新开始"));
		GetDlgItem(MIDC.STOP)->EnableWindow(false);
		GetDlgItem(MIDC.RESET)->EnableWindow(false);
		GetDlgItem(MIDC.TIP)->EnableWindow(false);
	}
	else if (nIDEvent == MIDC.TIMER) {
		m_GameC.timer.Increase();
		UpdateTimer();
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CGameRelaxDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CGameDlg::OnLButtonUp(nFlags, point);
}


void CGameRelaxDlg::OnBnClickedBtnHelpRelax()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(MIDC.TIMER);

	ShowMessage(
		"1. 点击的两个图案之间存在通路即可消去，消去后会根据图案间距获得积分\n"
		"2. 休闲模式没有时间限制，但是在游戏结束时需要支付时间消耗，一分钟内每10秒一积分，一分钟以后每秒一积分\n"
		"3. 使用提示、重排都需要消耗积分，积分不足时会赠送免费次数\n"
		"4. 若当前积分不足以支付时间消耗，则视为游戏失败\n"
		"5. 游戏成功后会记录在历史排行中，还可以在排行榜中查看其它用户的最好记录\n"
		, "帮助-休闲模式游戏规则");

	if (m_bPlaying)
		SetTimer(MIDC.TIMER, 1000, nullptr);
}


void CGameRelaxDlg::OnBnClickedBtnAutoRelax()
{
	// TODO: 在此添加控件通知处理程序代码
	CGameDlg::OnBnClickedBtnAutoBasic();
}
