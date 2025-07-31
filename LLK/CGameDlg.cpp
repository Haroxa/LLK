// CGameDlg.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "afxdialogex.h"
#include "CGameDlg.h"
#include "CGameControl.h"
#include "string"

// CGameDlg 对话框
IDC CGameDlg::MIDC = {
	IDC_BTN_START, IDC_BTN_STOP, IDC_BTN_TIP, IDC_BTN_RESET,
	IDC_TIMER, IDC_SCORE, IDC_LIST_BASIC, IDC_STATIC_AUTO_BASIC
};
CGameControl CGameDlg::m_GameC = {};

IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::CGameDlg(CWnd* pParent /*=nullptr*/, UINT ID)
	: CDialogEx(ID, pParent)
{
	m_FirstOpen = true;

	// 切换组件
	if (ID == IDD_GAME_DIALOG) {
		MIDC = {
			IDC_BTN_START, IDC_BTN_STOP, IDC_BTN_TIP, IDC_BTN_RESET,
			IDC_TIMER, IDC_SCORE, IDC_LIST_BASIC, IDC_BTN_HELP_BASIC,
			IDC_BTN_AUTO_BASIC, IDC_STATIC_AUTO_BASIC
		};
	}
	else if (ID == IDD_RELAX_DIALOG) {
		MIDC = {
			IDC_BTN_START_RELAX, IDC_BTN_STOP_RELAX, IDC_BTN_TIP_RELAX, 
			IDC_BTN_RESET_RELAX,
			IDC_TIMER_RELAX, IDC_SCORE_RELAX, IDC_LIST_RELAX, 
			IDC_BTN_HELP_RELAX,
			IDC_BTN_AUTO_RELAX, IDC_STATIC_AUTO_RELAX
		};
	}
}

CGameDlg::~CGameDlg()
{
	//delete m_TimerStr;	m_TimerStr = NULL;
	//delete m_ScoreStr;	m_ScoreStr = NULL;
	//delete m_ListStr;	m_ListStr = NULL;
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, MIDC.TIMER, m_TimerStr);
	DDX_Control(pDX, MIDC.SCORE, m_ScoreStr);
	DDX_Control(pDX, MIDC.LIST, m_ListStr);
	DDX_Control(pDX, MIDC.AUTONUM, m_AutoNumStr);
}


BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(MIDC.START, &CGameDlg::OnBnClickedBtnStart)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(MIDC.RESET, &CGameDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(MIDC.TIP, &CGameDlg::OnBnClickedBtnTip)
	ON_BN_CLICKED(MIDC.STOP, &CGameDlg::OnBnClickedBtnStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(MIDC.HELP, &CGameDlg::OnBnClickedBtnHelpBasic)
	ON_BN_CLICKED(MIDC.AUTO, &CGameDlg::OnBnClickedBtnAutoBasic)
END_MESSAGE_MAP()


// CGameDlg 消息处理程序

// 新增函数

void CGameDlg::InitBackground() {
	// 获得当前对话框的视频内存
	CClientDC dc(this);
	// 获取位图资源
	HANDLE hBmp = ::LoadImageW(NULL, _T("theme\\picture\\fruit_bg.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	// 创建与视频内存兼容的内存 DC
	m_dcMem.CreateCompatibleDC(&dc);
	// 将位图资源选入 DC
	m_dcMem.SelectObject(hBmp);
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


	HANDLE hBG = ::LoadImageW(NULL, _T("theme\\picture\\fruit_bg.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_dcBG.CreateCompatibleDC(&dc);
	m_dcBG.SelectObject(hBG);
}

void CGameDlg::InitElement(void)
{
	// 获得当前对话框的视频内存
	CClientDC dc(this);
	// 加载 BMP 图片资源
	HANDLE hBmp = ::LoadImageW(NULL, _T("theme\\picture\\fruit_element.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// 创建与视频内存兼容的内存 DC
	m_dcElement.CreateCompatibleDC(&dc);
	// 将位图资源选入 DC
	m_dcElement.SelectObject(hBmp);
	// 加载掩码 BMP 图片资源
	HANDLE hMask = ::LoadImageW(NULL, _T("theme\\picture\\fruit_mask.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	CBitmap bmpMask;
	bmpMask.Attach((HBITMAP)hMask);
	// 获取位图信息
	BITMAP bmpInfo;
	bmpMask.GetBitmap(&bmpInfo);
	// 计算每个图片的尺寸
	m_sizeElem.cx = bmpInfo.bmWidth;
	m_sizeElem.cy = bmpInfo.bmHeight / CGameControl::s_nPicNum;

	// 创建与视频内存兼容的内存 DC
	m_dcMask.CreateCompatibleDC(&dc);
	// 将位图资源选入 DC
	m_dcMask.SelectObject(hMask);
}

void CGameDlg::UpdateWindow() {
	// 调整窗口大小
	CRect rtWin;
	CRect rtClient;
	this->GetWindowRect(rtWin); // 获得窗口大小
	this->GetClientRect(rtClient); // 获得客户区大小
	// 标题栏和外边框的大小
	int nSpanWidth = rtWin.Width() - rtClient.Width();
	int nSpanHeight = rtWin.Height() - rtClient.Height();
	// 设置窗口大小
	MoveWindow(0, 0, BG::WIDTH + nSpanWidth, BG::HEIGHT + nSpanHeight);
	// 设置对话框显示是，在 windows 窗口正中央。
	CenterWindow();
}

void CGameDlg::UpdateMap() {
	// 计算图片的顶点坐标与图片大小
	int nLeft = m_ptGameTop.x, nTop = m_ptGameTop.y;
	int nElemW = m_sizeElem.cx, nElemH = m_sizeElem.cy;
	//获取行数和列数
	int nRows = CGameControl::s_nRows, nCols = CGameControl::s_nCols;
	// 初始化游戏区域大小
	m_rtGameRect = CRect(nLeft, nTop, nLeft + nCols * nElemW, 
		nTop + nRows * nElemH);

	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
		{
			// 得到图片编号的值
			int nElemVal = m_GameC.GetElement(i, j);
			m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemW, 
				nElemH, &m_dcBG, nLeft + j * nElemW, nTop + i * nElemH, SRCCOPY);
			if (nElemVal != BLANK) {
				// 将背景与掩码相或，边保留，图像区域为 1
				m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, 
					nElemW, nElemH, &m_dcMask, 0, nElemVal * nElemH, SRCPAINT);
				// 与元素图片相与，边保留，图像区域为元素图片
				m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemW, 
					nElemH, &m_dcElement, 0, nElemVal * nElemH, SRCAND);
			}
		}

}

void CGameDlg::UpdateList() {
	int num = m_GameC.nowUser.recordNum;
	Record* record = m_GameC.nowUser.record;
	int nowMode = m_GameC.mode;
	CString str;
	str.Format(L"%2s %2s  %4s",L"排名",L"积分",L"时间");
	m_ListStr.ResetContent();				// 清空列表框
	int i = 0;
	m_ListStr.InsertString(i++,str);
	while (record != NULL) {
		if (record->mode == nowMode) {
			str.Format(L"%02d %04d  %s", i, record->score, 
				(CString)record->time.c_str());
			m_ListStr.InsertString(i++, str);
		}
		record = record->next;
	}
}

void CGameDlg::UpdateTimer() {
	CString str;
	if(m_GameC.mode==m_GameC.BASIC)
		str.Format(L"倒计时：%02d:%02d:%02d", m_GameC.timer.hour,
			m_GameC.timer.min, m_GameC.timer.sec);
	else if (m_GameC.mode == m_GameC.RELAX)
		str.Format(L"计时器：%02d:%02d:%02d", m_GameC.timer.hour,
			m_GameC.timer.min, m_GameC.timer.sec);
	m_TimerStr.SetWindowTextW(str);
}
void CGameDlg::UpdateScore() {
	CString str;
	str.Format(L"当前积分数：%04d", m_GameC.score);
	m_ScoreStr.SetWindowTextW(str);
}


void CGameDlg::DrawTipFrame(int nRow, int nCol) {
	CClientDC dc(this);
	CPen penLine(PS_SOLID, 2, RGB(255, 0, 0));
	CPen* pOldPen = dc.SelectObject(&penLine);
	// 使用空画刷，即不填充矩形内部
	CBrush* pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);

	// 绘制矩形框
	int nLeft = m_ptGameTop.x + nCol * m_sizeElem.cx, 
		nRight = nLeft + m_sizeElem.cx;
	int nTop = m_ptGameTop.y + nRow * m_sizeElem.cy, 
		nBottom = nTop + m_sizeElem.cy;
	dc.Rectangle(nLeft, nTop, nRight, nBottom);
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}

void CGameDlg::DrawTipLine(Vertex avPath[], int nVexnum)
{
	CClientDC dc(this);
	CPen penLine(PS_SOLID, 2, RGB(0, 255, 0));
	CPen* pOldPen = dc.SelectObject(&penLine);
	// 绘制连接线
	dc.MoveTo(m_ptGameTop.x + avPath[0].col * m_sizeElem.cx + m_sizeElem.cx / 2,
		m_ptGameTop.y + avPath[0].row * m_sizeElem.cy + m_sizeElem.cy / 2);
	for (int i = 1; i < nVexnum; i++)
		dc.LineTo(m_ptGameTop.x + avPath[i].col*m_sizeElem.cx + m_sizeElem.cx/2,
			m_ptGameTop.y + avPath[i].row * m_sizeElem.cy + m_sizeElem.cy / 2);
	dc.SelectObject(pOldPen);
}

int CGameDlg::ShowMessage(const char* message, const char* title, int mode) {
	CString messageC(message), titleC(title);
	return MessageBox(messageC, titleC, mode);
}



void CGameDlg::SetListFont(CListBox* listBox, int fontSize) {
	listFont.CreatePointFont(fontSize, _T("Times New Roman"));
	listBox->SetFont(&listFont);
}


BOOL CGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_ptGameTop = CPoint(15, 45);
	m_bFirstPoint = true;
	m_bPlaying = false;
	m_GameC.SelectMode(m_GameC.BASIC);
	GetDlgItem(MIDC.START)->EnableWindow(!m_bPlaying);
	GetDlgItem(MIDC.STOP)->EnableWindow(m_bPlaying);
	GetDlgItem(MIDC.TIP)->EnableWindow(m_bPlaying);
	GetDlgItem(MIDC.RESET)->EnableWindow(m_bPlaying);
	InitBackground();
	InitElement();
	UpdateList();
	SetListFont(&m_ListStr, 61);
	UpdateTimer();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

bool IsNeedBasicHelp = true;
void CGameDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	// 绘制背景图片
	dc.BitBlt(0, 0, BG::WIDTH, BG::HEIGHT, &m_dcMem, 0, 0, SRCCOPY);
	if (IsNeedBasicHelp) {
		OnBnClickedBtnHelpBasic();
		IsNeedBasicHelp = false;
	}
}


void CGameDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	// 更新游戏区域
	m_bPlaying = true;
	GetDlgItem(MIDC.START)->EnableWindow(!m_bPlaying);
	GetDlgItem(MIDC.STOP)->EnableWindow(m_bPlaying);
	GetDlgItem(MIDC.TIP)->EnableWindow(m_bPlaying);
	GetDlgItem(MIDC.RESET)->EnableWindow(m_bPlaying);

	m_dcMem.BitBlt(m_ptGameTop.x, m_ptGameTop.y, m_rtGameRect.Width(), 
		m_rtGameRect.Height(), &m_dcBG, m_ptGameTop.x, m_ptGameTop.y, SRCCOPY);

	if (m_FirstOpen && m_GameC.ReadTempData()) {
		m_bPlaying = !(ShowMessage("检测到存在未完成对局，是否恢复？", 
			"恢复对局", MB_YESNO) == IDYES);
		m_FirstOpen = false;
	}
	if (m_bPlaying)
		m_GameC.StartGame();
	SetTimer(MIDC.TIMER,1000, nullptr);
	if (!m_bPlaying) {
		m_bPlaying = true;
		OnBnClickedBtnStop();
	}
	UpdateTimer();
	UpdateScore();
	UpdateMap();
	Invalidate();			// 更新游戏区域
}

void CGameDlg::ClearPoint(Vertex v1) {
	int nRow = v1.row, nCol = v1.col;
	int nLeft = m_ptGameTop.x + nCol * m_sizeElem.cx;
	int nTop = m_ptGameTop.y + nRow * m_sizeElem.cy;
	m_dcMem.BitBlt(nLeft, nTop, m_sizeElem.cx, m_sizeElem.cy, &m_dcBG, nLeft, nTop, SRCCOPY);
}

void CGameDlg::ShowGameOver() {
	CStringA ch;
	if (m_GameC.mode == m_GameC.BASIC)
		if(m_GameC.m_UseAuto)
			ch.Format("恭喜完成本次游戏！外挂使用次数：%02d，"
				"剩余时间转换数： % d\n" "本次对局总积分数为：%03d。", 
				m_GameC.m_UseAuto, m_GameC.timer.del, m_GameC.score);
		else
			ch.Format("恭喜完成本次游戏！剩余时间将全部转换为积分：%d，\n"
				"本次对局总积分数为：%03d。", m_GameC.timer.del, m_GameC.score);
	else if (m_GameC.mode == m_GameC.RELAX)
		ch.Format("恭喜完成本次游戏！将根据完成时间计算时间耗费：%d\n"
			"本次对局总积分数为：%03d。", -m_GameC.timer.del, m_GameC.score);
	ShowMessage(ch, "游戏结束");
}

void CGameDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_bPlaying)
		return;
	int nRow = (point.y - m_ptGameTop.y) / m_sizeElem.cy;
	int nCol = (point.x - m_ptGameTop.x) / m_sizeElem.cx;
	CDialogEx::OnLButtonUp(nFlags, point);
	// 判断鼠标点击的区域
	if (point.y < m_rtGameRect.top || point.y > m_rtGameRect.bottom
		|| point.x < m_rtGameRect.left || point.x > m_rtGameRect.right
		|| m_GameC.GetElement(nRow, nCol) == BLANK) {
		Invalidate();
		m_bFirstPoint = true;
		return CDialogEx::OnLButtonUp(nFlags, point);
	}
	if (m_bFirstPoint) // 第一个点
	{
		// 绘制矩形框
		m_GameC.SetFirstPoint(nRow, nCol);
		DrawTipFrame(nRow, nCol);
	}
	else // 第二个点
	{
		// 绘制矩形框
		m_GameC.SetSecPoint(nRow, nCol);
		DrawTipFrame(nRow, nCol);
		// 连子判断
		bool bSuc = m_GameC.Link(avPath, nVexnum);
		if (bSuc == true)
		{
			// 画提示线
			DrawTipLine(avPath, nVexnum);
			// 实现消除效果
			ClearPoint(m_GameC.m_svSelFst);
			ClearPoint(m_GameC.m_svSelSec);
			// 更新地图
			// 游戏结束
			if (m_GameC.IsWin()) {
				KillTimer(MIDC.TIMER);
				ShowGameOver();
				m_bPlaying = false;
				GetDlgItem(MIDC.START)->EnableWindow(true);
				GetDlgItem(MIDC.START)->SetWindowText(_T("重新开始"));
				GetDlgItem(MIDC.STOP)->EnableWindow(false);
				GetDlgItem(MIDC.RESET)->EnableWindow(false);
				GetDlgItem(MIDC.TIP)->EnableWindow(false);
				UpdateList();
			}
			UpdateScore();
		}
		UpdateMap();
		Sleep(150);
		Invalidate();
		m_GameC.ClearPoint();
	}
	m_bFirstPoint = !m_bFirstPoint;
}



void CGameDlg::OnBnClickedBtnReset()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(MIDC.TIMER);
	int opt = IDYES;
	if (m_GameC.score >= 10)
		opt = ShowMessage("是否消耗 10 积分进行重排？", "重排", MB_YESNO);
	else {
		m_GameC.score += 10;
		ShowMessage("积分过低赠送一次免费重排机会。", "重排");
	}
	if (opt == IDYES) {
		m_GameC.score -= 10;
		UpdateScore();
		m_GameC.ResetMap();
		UpdateMap();
		Invalidate();
	}
	SetTimer(MIDC.TIMER, 1000, nullptr);
}


void CGameDlg::OnBnClickedBtnTip()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(MIDC.TIMER);
	int opt = IDYES;
	if (m_GameC.score >= 3)
		opt = ShowMessage("是否消耗 3 积分获取提示？", "提示", MB_YESNO);
	else {
		m_GameC.score += 3;
		ShowMessage("积分过低赠送一次免费提示机会。", "提示");
	}
	if (opt == IDYES) {
		m_GameC.score -= 3;
		UpdateScore();
		m_GameC.GetTip(avPath, nVexnum);
		DrawTipFrame(avPath[0].row, avPath[0].col);
		DrawTipFrame(avPath[nVexnum - 1].row, avPath[nVexnum - 1].col);
		DrawTipLine(avPath, nVexnum);
		Sleep(300);
		Invalidate();
	}
	SetTimer(MIDC.TIMER, 1000, nullptr);
}

void CGameDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bPlaying) {
		GetDlgItem(MIDC.START)->EnableWindow(m_bPlaying);
		GetDlgItem(MIDC.START)->SetWindowText(_T("重新开始"));
		GetDlgItem(MIDC.STOP)->SetWindowText(_T("继续游戏"));
		KillTimer(MIDC.TIMER);
	}
	else {
		GetDlgItem(MIDC.START)->EnableWindow(m_bPlaying);
		GetDlgItem(MIDC.START)->SetWindowText(_T("开始游戏"));
		GetDlgItem(MIDC.STOP)->SetWindowText(_T("暂停游戏"));
		SetTimer(MIDC.TIMER, 1000 , nullptr);
	}
	m_bPlaying = !m_bPlaying;
}


void CGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_GameC.timer.IsZero()) {
		KillTimer(MIDC.TIMER);
		ShowMessage("游戏时间到，未能消除所有图案！", "游戏失败");
		GetDlgItem(MIDC.START)->EnableWindow(true);
		GetDlgItem(MIDC.START)->SetWindowText(_T("重新开始"));
		GetDlgItem(MIDC.STOP)->EnableWindow(false);
		GetDlgItem(MIDC.RESET)->EnableWindow(false);
		GetDlgItem(MIDC.TIP)->EnableWindow(false);
	}
	else if (nIDEvent == MIDC.TIMER) {
		m_GameC.timer.Decrease();
		UpdateTimer();
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CGameDlg::OnBnClickedBtnHelpBasic()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(MIDC.TIMER);
	ShowMessage(
		"1. 点击的两个图案之间存在通路即可消去，消去后会获得 2 积分\n"
		"2. 请在10分钟内消除所有图案，剩余时间会全部转换为积分，使用外挂后会按使用次数进行一定比例的削减\n"
		"3. 使用提示、重排都需要消耗积分，积分不足时会赠送免费次数\n"
		"4. 若倒计时结束未消除所有图案，则视为游戏失败\n"
		"5. 游戏成功后会记录在历史排行中，还可以在排行榜中查看其它用户的最好记录\n"
		,"帮助-基本模式游戏规则");
	if (m_bPlaying)
		SetTimer(MIDC.TIMER, 1000, nullptr);
}

void CGameDlg::OnBnClickedBtnAutoBasic()
{
	// TODO: 在此添加控件通知处理程序代码
	static auto VetexToCPoint = [&](Vertex& v) {
		int x = v.col * m_sizeElem.cx + m_ptGameTop.x;
		int y = v.row * m_sizeElem.cy + m_ptGameTop.y;
		return CPoint(x, y);
	};
	CString str;
	m_AutoNumStr.GetWindowText(str);
	int cnt;
	try {
		cnt = std::stoi(static_cast<LPCTSTR>(str));
	}
	catch(const std::invalid_argument& e){
		str = "10", cnt = 10;
	}
	if (cnt < 0)
		str = "10", cnt = 10;
	while (m_bPlaying && cnt--) {
		m_GameC.m_UseAuto++;
		m_GameC.GetTip(avPath, nVexnum);
		Vertex v1 = avPath[0], v2 = avPath[nVexnum - 1];
		OnLButtonUp(0, VetexToCPoint(v1));
		OnLButtonUp(0, VetexToCPoint(v2));
		Sleep(150);
		OnPaint();
	}
	m_AutoNumStr.SetWindowText(str);
}

