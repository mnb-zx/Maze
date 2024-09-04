
// MazeGameDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MazeGame.h"
#include "MazeGameDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CMazeGameDlg 对话框





void CMazeGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMazeGameDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMazeGameDlg 消息处理程序

BOOL CMazeGameDlg::OnInitDialog()
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
	// 
	// 确保窗口句柄已经初始化后再调用 Invalidate()
	Invalidate();

	// 初始化计时器
	m_nSecondsElapsed = 0;
	SetTimer(1, 1000, nullptr); // 每秒触发一次

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMazeGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

//void CMazeGameDlg::OnPaint()
//{
//	if (IsIconic())
//	{
//		CPaintDC dc(this); // 用于绘制的设备上下文
//
//		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
//
//		// 使图标在工作区矩形中居中
//		int cxIcon = GetSystemMetrics(SM_CXICON);
//		int cyIcon = GetSystemMetrics(SM_CYICON);
//		CRect rect;
//		GetClientRect(&rect);
//		int x = (rect.Width() - cxIcon + 1) / 2;
//		int y = (rect.Height() - cyIcon + 1) / 2;
//
//		// 绘制图标
//		dc.DrawIcon(x, y, m_hIcon);
//	}
//	else
//	{
//		CDialogEx::OnPaint();
//	}
//}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMazeGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CMazeGameDlg::CMazeGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAZEGAME_DIALOG, pParent), m_pMaze(nullptr), m_mazeSize(32)
{
}

void CMazeGameDlg::InitializeMaze(int size)
{
	m_mazeSize = size;
	if (m_pMaze) {
		delete m_pMaze;
	}
	m_pMaze = new Maze(m_mazeSize, m_mazeSize);
	m_pMaze->generate();
	m_playerPos = m_pMaze->getStart();
}

void CMazeGameDlg::OnPaint()
{
	CPaintDC dc(this);
	if (!m_pMaze) {
		TRACE("m_pMaze is null in OnPaint\n");
		return;
	}
	CRect clientRect;
	GetClientRect(&clientRect);
	int cellSize = min(clientRect.Width() / m_mazeSize, clientRect.Height() / m_mazeSize);

	for (int y = 0; y < m_mazeSize; ++y) {
		for (int x = 0; x < m_mazeSize; ++x) {
			CRect cellRect(x * cellSize, y * cellSize, (x + 1) * cellSize, (y + 1) * cellSize);
			if (m_pMaze->isWall(x, y)) {
				dc.FillSolidRect(cellRect, RGB(0, 0, 0)); // 黑色墙壁
			}
			else {
				dc.FillSolidRect(cellRect, RGB(255, 255, 255)); // 白色路径
			}
		}
	}

	auto start = m_pMaze->getStart();
	CRect startRect(start.first * cellSize, start.second * cellSize, (start.first + 1) * cellSize, (start.second + 1) * cellSize);
	dc.FillSolidRect(startRect, RGB(0, 255, 0)); // 绿色起点

	auto end = m_pMaze->getEnd();
	CRect endRect(end.first * cellSize, end.second * cellSize, (end.first + 1) * cellSize, (end.second + 1) * cellSize);
	dc.FillSolidRect(endRect, RGB(255, 0, 0)); // 红色终点

	CRect playerRect(m_playerPos.first * cellSize, m_playerPos.second * cellSize, (m_playerPos.first + 1) * cellSize, (m_playerPos.second + 1) * cellSize);
	dc.FillSolidRect(playerRect, RGB(0, 0, 255)); // 蓝色玩家

	// 绘制提示文字
	CString strHint;
	if (m_mazeSize == 32) {
		strHint = _T("2分钟内通关");
	}
	else if (m_mazeSize == 64) {
		strHint = _T("4分钟内通关");
	}

	int hintX = 0;
	int hintY = 0;
	if (!strHint.IsEmpty()) {
		// 设置提示文字的字体大小为计时器字体大小的一半
		CFont hintFont;
		hintFont.CreatePointFont(80, _T("Arial")); // 80 = 160 / 2
		CFont* pOldHintFont = dc.SelectObject(&hintFont);

		// 获取提示文字的宽度和高度
		CSize hintTextSize = dc.GetTextExtent(strHint);
		int mazeRightWallX = m_mazeSize * cellSize; // 迷宫右壁的X坐标
		int dialogRightBorderX = clientRect.right; // 对话框右边框的X坐标
		int timerCenterX = (mazeRightWallX + dialogRightBorderX) / 2; // 计算正中间的X坐标
		hintX = timerCenterX - hintTextSize.cx / 2; // 计算提示文字左上角的X坐标
		hintY = 4 * cellSize - hintTextSize.cy - 2; // 计算提示文字的Y坐标

		// 如果是64*64迷宫，向下平移6个单位
		if (m_mazeSize == 64) {
			hintY += 6 * cellSize;
		}

		// 设置背景透明
		dc.SetBkMode(TRANSPARENT);
		dc.TextOutW(hintX, hintY, strHint); // 绘制提示文字

		// 恢复原来的字体
		dc.SelectObject(pOldHintFont);
	}

	// 绘制计时器
	CString strTime;
	strTime.Format(_T("%02d:%02d"), m_nSecondsElapsed / 60, m_nSecondsElapsed % 60);
	CSize textSize = dc.GetTextExtent(strTime); // 获取文本的宽度和高度
	int timerX = hintX; // ! 将计时器的X坐标设置为提示文字的X坐标
	int timerY = 4 * cellSize; // 向下平移4个单位

	// 如果是64*64迷宫，向下平移6个单位
	if (m_mazeSize == 64) {
		timerY += 6 * cellSize;
	}

	// 设置字体大小为原来的4倍
	CFont font;
	font.CreatePointFont(160, _T("Arial")); // 160 = 4 * 40 (默认字体大小为40)
	CFont* pOldFont = dc.SelectObject(&font);

	// 设置背景透明
	dc.SetBkMode(TRANSPARENT);

	// 根据时间和迷宫大小设置字体颜色和跳动特效
	bool isBlinking = false;
	if ((m_mazeSize == 32 && m_nSecondsElapsed >= 110) || (m_mazeSize == 64 && m_nSecondsElapsed >= 230)) {
		dc.SetTextColor(RGB(255, 0, 0)); // 红色字体
		isBlinking = true;
	}
	else {
		dc.SetTextColor(RGB(0, 0, 0)); // 黑色字体
	}

	// 如果需要跳动特效
	if (isBlinking && (m_nSecondsElapsed % 2 == 0)) {
		font.DeleteObject();
		font.CreatePointFont(300, _T("Arial")); // 增大字体
		dc.SelectObject(&font);
	}

	dc.TextOutW(timerX, timerY, strTime); // 修改X坐标

	// 恢复原来的字体
	dc.SelectObject(pOldFont);
}


void CMazeGameDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
	case VK_UP:    MovePossible(0, -1); break;
	case VK_DOWN:  MovePossible(0, 1);  break;
	case VK_LEFT:  MovePossible(-1, 0); break;
	case VK_RIGHT: MovePossible(1, 0);  break;
	}
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMazeGameDlg::MovePossible(int dx, int dy)
{
	int newX = m_playerPos.first + dx;
	int newY = m_playerPos.second + dy;
	if (!m_pMaze->isWall(newX, newY)) {
		m_playerPos = { newX, newY };
		Invalidate();
	}
}

BOOL CMazeGameDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		OnKeyDown(pMsg->wParam, pMsg->lParam, 0);
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CMazeGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1) {
		m_nSecondsElapsed++;
		Invalidate(); // 触发重绘
	}
	CDialogEx::OnTimer(nIDEvent);
}