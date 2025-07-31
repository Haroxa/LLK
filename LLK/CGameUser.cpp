// CGameUser.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "afxdialogex.h"
#include "CGameUser.h"
#include "CGameControl.h"
#include "CGameDlg.h"

// CGameUser 对话框

IMPLEMENT_DYNAMIC(CGameUser, CDialogEx)

CGameUser::CGameUser(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USER_DIALOG, pParent)
{

}

CGameUser::~CGameUser()
{
}

void CGameUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USERNAME, UsernameStr);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, PasswordStr);
}


BEGIN_MESSAGE_MAP(CGameUser, CDialogEx)
	ON_BN_CLICKED(IDC_LOGIN_OK, &CGameUser::OnBnClickedLoginOk)
END_MESSAGE_MAP()


// CGameUser 消息处理程序
void CGameUser::InitWindow() {
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

BOOL CGameUser::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

int CGameUser::ShowMessage(const char* message, const char* title, int mode) {
	CString messageC(message), titleC(title);
	return MessageBox(messageC, titleC, mode);
}


void CGameUser::OnBnClickedLoginOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CGameControl::nowUser.status = m_bUserStatus;
	if (!m_bUserStatus) {
		CString une, pwd;
		UsernameStr.GetWindowText(une);
		PasswordStr.GetWindowText(pwd);
		username = CStringA(une);
		password = CStringA(pwd);
		if (!User::Check(username, "[0-9a-zA-Z]{3,7}")) {
			ShowMessage("用户名应为3~7位的数字字符", "用户名格式错误");
		}
		else if (!User::Check(password, "[0-9a-zA-Z]{8,16}")) {
			ShowMessage("密码应为8~16位的数字字符", "密码格式错误");
		}
		else {
			User user = CGameControl::userMg.Query({ username });
			int opt = IDYES;
			if (!user.username.empty()) {
				if(user.password==password)
					ShowMessage("检测到用户存在，登录验证成功", "登录");
				else {
					opt = IDNO;
					ShowMessage("检测到用户存在，登录验证失败", "登录");
				}
			}
			else {
				opt = ShowMessage("检测到用户不存在，是否进行注册？", 
					"注册", MB_YESNO);
				if (opt == IDYES) {
					user = { username, password };
					CGameControl::userMg.Add(user);
				}
			}
			if (opt == IDYES) {
				CGameDlg::m_GameC.Login(user);
				EndDialog(opt);
			}
		}
	}
}
