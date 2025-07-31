
// LLKDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "LLK.h"
#include "LLKDlg.h"
#include "afxdialogex.h"
#include "CGameDlg.h"
#include "CGameSetting.h"
#include "CGameUser.h"
#include "CGameRelaxDlg.h"
#include "CGameRankDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLLKDlg 对话框



CLLKDlg::CLLKDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LLK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLLKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATUS, UserStatusStr);
}

BEGIN_MESSAGE_MAP(CLLKDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_BASIC, &CLLKDlg::OnBnClickedBtnBasic)
	ON_BN_CLICKED(IDC_BTN_SETTING, &CLLKDlg::OnClickedBtnSetting)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CLLKDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_RELAX, &CLLKDlg::OnBnClickedBtnRelax)
	ON_BN_CLICKED(IDC_BTN_RANK, &CLLKDlg::OnBnClickedBtnRank)
	ON_BN_CLICKED(IDC_BTN_HELP, &CLLKDlg::OnBnClickedBtnHelp)
END_MESSAGE_MAP()


// CLLKDlg 消息处理程序

BOOL CLLKDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitBackground();
	CString str;
	str.Format(L"当前状态：未登录");
	UserStatusStr.SetWindowTextW(str);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLLKDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLLKDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// 创建 CPaintDC 对象
		CPaintDC dc(this);
		// 绘制背景图片
		dc.BitBlt(0, 0, BG::WIDTH, BG::HEIGHT, &m_dcMem, 0, 0, SRCCOPY);
		CDialogEx::OnPaint();
		
		OnBnClickedBtnHelp();

	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLLKDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 新增函数


void CLLKDlg::InitBackground() {
	// 获得当前对话框的视频内存
	CClientDC dc(this);
	// 获取位图资源
	CBitmap bmpMain;
	bmpMain.LoadBitmapW(IDB_MAIN_BG);
	// 创建与视频内存兼容的内存 DC
	m_dcMem.CreateCompatibleDC(&dc);
	// 将位图资源选入 DC
	m_dcMem.SelectObject(bmpMain);
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

void CLLKDlg::OnBnClickedBtnBasic()
{
	// TODO: 在此添加控件通知处理程序代码
	CGameDlg dlg;
	dlg.DoModal();
}



void CLLKDlg::OnClickedBtnSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	CGameSetting dlg;
	dlg.DoModal();
}


int CLLKDlg::ShowMessage(const char* message, const char* title, int mode) {
	CString messageC(message), titleC(title);
	return MessageBox(messageC, titleC, mode);
}

void CLLKDlg::OnBnClickedBtnLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	if (CGameControl::nowUser.status == 1) {
		int opt = ShowMessage("是否确认退出登录？", "退出",MB_YESNO);
		if (opt == IDYES) {
			CGameDlg::m_GameC.Logout();
			CString str;
			str.Format(L"当前状态：未登录");
			UserStatusStr.SetWindowTextW(str);
			GetDlgItem(IDC_BTN_LOGIN)->SetWindowText(_T("登录"));
		}
		return;
	}
	else {
		CGameUser dlg;
		dlg.DoModal();
		if (CGameControl::nowUser.status==1) {
			CString str;
			str.Format(L"欢迎回来：%s", 
				(CString)(CGameControl::nowUser.username.c_str()));
			UserStatusStr.SetWindowTextW(str);
			GetDlgItem(IDC_BTN_LOGIN)->SetWindowText(_T("退出"));

		}
	}
}


void CLLKDlg::OnBnClickedBtnRelax()
{
	// TODO: 在此添加控件通知处理程序代码
	CGameRelaxDlg dlg;
	dlg.DoModal();
}


void CLLKDlg::OnBnClickedBtnRank()
{
	// TODO: 在此添加控件通知处理程序代码
	CGameRankDlg dlg;
	dlg.DoModal();
}


void CLLKDlg::OnBnClickedBtnHelp()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowMessage(
		"1. 欢迎使用欢乐连连看，本游戏目前包括基本模式和休闲模式，"
		"具体规则请查看对应帮助内容\n"
		"2. 登录后会保持之前的游戏记录，并且可以查看总的排行榜\n"
		"3. 用户密码需要符合对应的格式，如果用户不存在则视为注册用户\n"
		"4. 其它功能开发中...\n"
		, "帮助-游戏简介");
}
