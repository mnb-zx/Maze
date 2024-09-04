
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
	ON_BN_CLICKED(IDC_PAUSE_BUTTON, &CMazeGameDlg::OnPauseButtonClicked) // 添加按钮点击事件映射
END_MESSAGE_MAP()




// CMazeGameDlg 消息处理程序

// MazeGameDlg.cpp

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

	// 初始化标记
	m_bNeedUpdateWalls = true;

	CRect clientRect;
	GetClientRect(&clientRect);
	CClientDC dc(this);
	m_memDC.CreateCompatibleDC(&dc);
	m_memBitmap.CreateCompatibleBitmap(&dc, clientRect.Width(), clientRect.Height());
	m_memDC.SelectObject(&m_memBitmap);

	m_prevPlayerPos = m_playerPos;

	// 初始化暂停按钮
	GetClientRect(&clientRect);
	int cellSize = min(clientRect.Width() / m_mazeSize, clientRect.Height() / m_mazeSize);
	int mazeRightEdge = m_mazeSize * cellSize; // 迷宫右边界的X坐标
	int dialogRightEdge = clientRect.right; // 对话框右边框的X坐标
	int pauseButtonX = (mazeRightEdge + dialogRightEdge) / 2 - 50; // 计算X坐标，假设按钮宽度为100
	int pauseButtonY = clientRect.Height() / 2 - 15; // 计算Y坐标，假设按钮高度为30

	m_pauseButton.Create(_T("暂停游戏"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(pauseButtonX, pauseButtonY, pauseButtonX + 100, pauseButtonY + 30), this, IDC_PAUSE_BUTTON);
	m_pauseButton.ShowWindow(SW_SHOW);

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
		CDialogEx::OnSysCommand(nID, static_cast<UINT>(lParam));
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMazeGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CMazeGameDlg::CMazeGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAZEGAME_DIALOG, pParent), m_pMaze(nullptr), m_mazeSize(32), m_bTimerRunning(false), m_nSecondsElapsed(0), m_hIcon(nullptr)
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
	m_nSecondsElapsed = 0;
	m_bTimerRunning = true;
	SetTimer(1, 1000, nullptr); // 启动计时器
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

	if (m_bNeedUpdateWalls) {
		m_memDC.FillSolidRect(&clientRect, RGB(255, 255, 255)); // 清空背景
		for (int y = 0; y < m_mazeSize; ++y) {
			for (int x = 0; x < m_mazeSize; ++x) {
				CRect cellRect(x * cellSize, y * cellSize, (x + 1) * cellSize, (y + 1) * cellSize);
				if (m_pMaze->isWall(x, y)) {
					m_memDC.FillSolidRect(cellRect, RGB(0, 0, 0)); // 黑色墙壁
				}
				else {
					m_memDC.FillSolidRect(cellRect, RGB(255, 255, 255)); // 白色路径
				}
			}
		}
		m_bNeedUpdateWalls = false; // 重绘完墙体后设置为false
	}

	// 将内存设备上下文中的内容绘制到屏幕上
	dc.BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &m_memDC, 0, 0, SRCCOPY);

	// 只重绘上一个位置和当前的位置
	CRect prevPlayerRect(m_prevPlayerPos.first * cellSize, m_prevPlayerPos.second * cellSize, (m_prevPlayerPos.first + 1) * cellSize, (m_prevPlayerPos.second + 1) * cellSize);
	dc.FillSolidRect(prevPlayerRect, RGB(255, 255, 255)); // 白色路径

	CRect playerRect(m_playerPos.first * cellSize, m_playerPos.second * cellSize, (m_playerPos.first + 1) * cellSize, (m_playerPos.second + 1) * cellSize);
	dc.FillSolidRect(playerRect, RGB(0, 0, 255)); // 蓝色玩家

	auto start = m_pMaze->getStart();
	CRect startRect(start.first * cellSize, start.second * cellSize, (start.first + 1) * cellSize, (start.second + 1) * cellSize);
	dc.FillSolidRect(startRect, RGB(0, 255, 0)); // 绿色起点

	auto end = m_pMaze->getEnd();
	CRect endRect(end.first * cellSize, end.second * cellSize, (end.first + 1) * cellSize, (end.second + 1) * cellSize);
	dc.FillSolidRect(endRect, RGB(255, 0, 0)); // 红色终点

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
	dc.TextOutW(timerX, timerY, strTime); // 绘制计时器

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

// 在updateWall函数中更新内存设备上下文
void CMazeGameDlg::updateWall()
{
	m_bNeedUpdateWalls = true;
	Invalidate(); // 触发重绘
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

		// 起点重置计时器
		if (m_playerPos == m_pMaze->getStart()) {
			m_nSecondsElapsed = 0;
			m_bTimerRunning = true;
			SetTimer(1, 1000, nullptr); // 启动计时器
		}
		//终点停止计时器
		if (m_playerPos == m_pMaze->getEnd()) {
			m_bTimerRunning = false;
			KillTimer(1); // 停止计时器
		}
	}
}

BOOL CMazeGameDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (!m_bTimerRunning) {
			return TRUE; // 如果游戏暂停，不处理键盘事件
		}
		OnKeyDown(pMsg->wParam, pMsg->lParam, 0);
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMazeGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1 && m_bTimerRunning) {
		m_nSecondsElapsed++;
		Invalidate(); // 触发重绘
	}
	CDialogEx::OnTimer(nIDEvent);
}

int CMazeGameDlg::GetElapsedTime() const
{
	return m_nSecondsElapsed;
}


void CMazeGameDlg::OnPauseButtonClicked()
{
	// 暂停按钮点击处理逻辑
	if (m_bTimerRunning) {
		KillTimer(1); // 停止计时器
		m_bTimerRunning = false;
		m_pauseButton.SetWindowText(_T("继续游戏"));

		// 在暂停按钮下方3个单位处显示“游戏已暂停”
		CRect buttonRect;
		m_pauseButton.GetWindowRect(&buttonRect);
		ScreenToClient(&buttonRect);
		int textX = buttonRect.left;
		int textY = buttonRect.bottom + 3 * 15; // 3个单位处，假设单位高度为15

		CClientDC dc(this);
		dc.SetBkMode(TRANSPARENT);
		dc.TextOutW(textX, textY, _T("游戏已暂停"));
	}
	else {
		SetTimer(1, 1000, nullptr); // 启动计时器
		m_bTimerRunning = true;
		m_pauseButton.SetWindowText(_T("暂停游戏"));

		// 清除“游戏已暂停”文字
		Invalidate();
	}
}



