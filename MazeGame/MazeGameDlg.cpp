// MazeGameDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MazeGame.h"
#include "MazeGameDlg.h"
#include "afxdialogex.h"
#include <chrono>
#include <ctime>
#include"CEndGame.h"
#include<fstream>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#define FILENAME "TimeRecords.txt"
#endif
using namespace std;

CMazeGameDlg* pMazeGameDlg = nullptr;

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
    DDX_Control(pDX, IDC_TIMER, m_timerEdit); // 绑定计时器编辑控件
    DDX_Control(pDX, IDC_PASSREQUIRE_STATIC, m_passRequireStatic); // 绑定静态文本控件
    DDX_Control(pDX, IDC_PAUSE_BUTTON, m_pauseButton); // 绑定暂停按钮
    DDX_Control(pDX, IDC_SHOWPAUSE_STATIC, m_showPauseStatic); // 绑定显示暂停状态的静态文本
}


BEGIN_MESSAGE_MAP(CMazeGameDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER() // 添加计时器消息映射
    ON_WM_CTLCOLOR() // 添加控件颜色消息映射
    ON_BN_CLICKED(IDC_PAUSE_BUTTON, &CMazeGameDlg::OnBnClickedPauseButton)
END_MESSAGE_MAP()



// CMazeGameDlg 消息处理程序

BOOL CMazeGameDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    InitDatabase();
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
    SetIcon(m_hIcon, TRUE);            // 设置大图标
    SetIcon(m_hIcon, FALSE);        // 设置小图标

    // TODO: 在此添加额外的初始化代码
    // 
    // 确保窗口句柄已经初始化后再调用 Invalidate()
    Invalidate();

    // 初始化标记
    m_bNeedUpdateWalls = true;

    CRect clientRect;
    GetClientRect(&clientRect);
    CClientDC dc(this);
    m_memDC.CreateCompatibleDC(&dc);
    m_memBitmap.CreateCompatibleBitmap(&dc, clientRect.Width(), clientRect.Height());
    m_memDC.SelectObject(&m_memBitmap);

    m_prevPlayerPos = m_playerPos;

    // 初始化计时器
    m_startTime = CTime::GetCurrentTime();
    SetTimer(1, 1000, nullptr); // 每秒触发一次

    // 如果迷宫大小为64*64，设置静态文本框的文字
    if (m_mazeSize == 64) {
        m_passRequireStatic.SetWindowText(_T("4分钟内通关"));
    }

    m_isPaused = false; // 初始化为未暂停状态
    m_showPauseStatic.ShowWindow(SW_HIDE); // 初始化为隐藏状态
    m_isTimerAlert = false; // 初始化为未警告状态

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
    : CDialogEx(IDD_MAZEGAME_DIALOG, pParent), m_pMaze(nullptr), m_mazeSize(32), m_hIcon(nullptr),
    m_bNeedUpdateWalls(true), m_isPaused(false), m_isTimerAlert(false), m_isGameEnded(false),
    m_mazeData(32, std::vector<int>(32, 0)) 
{
    pMazeGameDlg = this;
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
}

// 在updateWall函数中更新内存设备上下文
void CMazeGameDlg::updateWall()
{
    m_bNeedUpdateWalls = true;
    Invalidate(); // 触发重绘
}

void CMazeGameDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (m_isPaused || m_isGameEnded) return; // 如果游戏暂停或已经结束，直接返回

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
    m_playerPath.emplace_back(m_playerPos.first, m_playerPos.second);
    int newX = m_playerPos.first + dx;
    int newY = m_playerPos.second + dy;
    if (!m_pMaze->isWall(newX, newY)) {
        m_playerPos = { newX, newY };
        Invalidate();
        // 检查是否到达终点
        if (m_playerPos == m_pMaze->getEnd()) {
            EndGame();
        }
    }
}

BOOL CMazeGameDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        OnKeyDown(static_cast<UINT>(pMsg->wParam), static_cast<UINT>(pMsg->lParam), 0);
        return TRUE;
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}


void CMazeGameDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1) {
        m_currentTime = CTime::GetCurrentTime();
        UpdateTimerDisplay();
    }
    else if (nIDEvent == 2) {
        // 跳动特效
        static bool toggle = false;
        CRect rect;
        m_timerEdit.GetWindowRect(&rect);
        ScreenToClient(&rect);
        if (toggle) {
            rect.OffsetRect(2, 2);
        }
        else {
            rect.OffsetRect(-2, -2);
        }
        m_timerEdit.MoveWindow(&rect);
        toggle = !toggle;
    }
    CDialogEx::OnTimer(nIDEvent);
}


void CMazeGameDlg::UpdateTimerDisplay()
{
    CTimeSpan elapsedTime = m_currentTime - m_startTime - m_totalPausedTime; // 减去总的暂停时间
    CString strTime;
    strTime.Format(_T("%02d:%02d"), elapsedTime.GetMinutes(), elapsedTime.GetSeconds());
    m_timerEdit.SetWindowText(strTime);

    // 检查是否需要进入警告状态
    if ((m_mazeSize == 32 && elapsedTime.GetTotalSeconds() >= 110) ||
        (m_mazeSize == 64 && elapsedTime.GetTotalSeconds() >= 230)) {
        SetTimerAlert(true);
    }
    else {
        SetTimerAlert(false);
    }
}

void CMazeGameDlg::EndGame()
{
    KillTimer(1);
    m_isGameEnded = true;
    CTimeSpan elapsed = m_currentTime - m_startTime;
    const_cast<CMazeGameDlg*>(this)->m_timeRecords.push_back(static_cast<int>(elapsed.GetTotalSeconds()));
    SaveTimeRecordsToFile();
    UpdateExitTime();

    // 保存玩家路径到文件
    std::ofstream file("PlayerPath.txt");
    for (const auto& pos : m_playerPath)
    {
        file << pos.first << " " << pos.second << std::endl;
    }

    CEndGame endGameDlg;
    endGameDlg.DoModal();
}

HBRUSH CMazeGameDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    if (pWnd->GetDlgCtrlID() == IDC_PASSREQUIRE_STATIC || pWnd->GetDlgCtrlID() == IDC_SHOWPAUSE_STATIC)
    {
        pDC->SetBkMode(TRANSPARENT); // 设置背景模式为透明
        return (HBRUSH)GetStockObject(NULL_BRUSH); // 返回空刷子
    }

    if (pWnd->GetDlgCtrlID() == IDC_TIMER && m_isTimerAlert)
    {
        pDC->SetTextColor(RGB(255, 0, 0)); // 设置字体颜色为红色
    }

    return hbr;
}


void CMazeGameDlg::PauseGame()
{
    m_isPaused = true;
    KillTimer(1); // 停止计时器
    m_pauseButton.SetWindowText(_T("继续游戏")); // 更新按钮文字
    m_showPauseStatic.ShowWindow(SW_SHOW); // 显示暂停状态的静态文本框
    m_pauseTime = CTime::GetCurrentTime(); // 记录暂停时的时间
}


void CMazeGameDlg::ResumeGame()
{
    m_isPaused = false;
    CTime currentTime = CTime::GetCurrentTime();
    m_totalPausedTime += currentTime - m_pauseTime; // 累加暂停的时间
    SetTimer(1, 1000, nullptr); // 重新启动计时器
    m_pauseButton.SetWindowText(_T("暂停游戏")); // 更新按钮文字
    m_showPauseStatic.ShowWindow(SW_HIDE); // 隐藏暂停状态的静态文本框
}


void CMazeGameDlg::OnBnClickedPauseButton()
{
    if (m_isPaused) {
        ResumeGame();
    }
    else {
        PauseGame();
    }
}


void CMazeGameDlg::SetTimerAlert(bool alert)
{
    if (alert && !m_isTimerAlert) {
        m_isTimerAlert = true;
        m_alertFont.CreatePointFont(200, _T("Arial Bold")); // 创建警告字体
        m_timerEdit.SetFont(&m_alertFont); // 设置计时器字体

        // 添加跳动特效
        CRect rect;
        m_timerEdit.GetWindowRect(&rect);
        ScreenToClient(&rect);
        SetTimer(2, 500, nullptr); // 每0.5秒触发一次
    }
    else if (!alert && m_isTimerAlert) {
        m_isTimerAlert = false;
        m_timerEdit.SetFont(nullptr); // 恢复默认字体
        KillTimer(2); // 停止跳动特效
    }
    m_timerEdit.Invalidate(); // 触发重绘以更新颜色
}

// 将CTime转换为std::chrono::system_clock::time_point
std::chrono::system_clock::time_point CTimeToChrono(const CTime& time)
{
    SYSTEMTIME st;
    time.GetAsSystemTime(st);

    struct tm tm;
    tm.tm_year = st.wYear - 1900;
    tm.tm_mon = st.wMonth - 1;
    tm.tm_mday = st.wDay;
    tm.tm_hour = st.wHour;
    tm.tm_min = st.wMinute;
    tm.tm_sec = st.wSecond;
    tm.tm_isdst = -1;

    auto timeT = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(timeT);
}

int CMazeGameDlg::GetTime() const
{
    auto startTimeChrono = CTimeToChrono(m_startTime);
    auto currentTimeChrono = CTimeToChrono(m_currentTime);

    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTimeChrono - startTimeChrono).count();
    return static_cast<int>(elapsed);
}


void CMazeGameDlg::SaveTimeRecordsToFile()
{
    std::sort(m_timeRecords.begin(), m_timeRecords.end(), std::greater<int>());
    std::ofstream ofs(FILENAME, std::ios::out);
    if (!ofs.is_open()) {
        AfxMessageBox(_T("无法打开文件进行写入"));
        return;
    }
    for (int time : m_timeRecords) {
        ofs << time << std::endl;
    }
    ofs.close();
}

void CMazeGameDlg::UpdateExitTime()
{
    // 获取游戏时间
    int gameTime = GetTime();

    // 将游戏时间转换为字符串
    CString strGameTime;
    strGameTime.Format(_T("%d"), gameTime);

    // 获取IDC_EXIT控件并设置其文本
    CEdit* pExitEdit = (CEdit*)GetDlgItem(IDC_SCOREDIT);
    if (pExitEdit != nullptr)
    {
        pExitEdit->SetWindowText(strGameTime);
    }
}

// 获取迷宫数据的实现
std::vector<std::vector<int>> CMazeGameDlg::GetMazeData() const {
    return m_pMaze->GetMazeData();
}

// 获取玩家路径的实现
std::vector<std::pair<int, int>> CMazeGameDlg::GetPlayerPath() const {
    return m_playerPath;
}

//创建数据库和表
void CMazeGameDlg::InitDatabase()
{
    sqlite3* DB;
    char* errorMessage;

    int exit = sqlite3_open("MazeGame.db", &DB);
    if (exit) {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    std::string sql = "CREATE TABLE IF NOT EXISTS GameRecords("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "StartTime TEXT NOT NULL, "
        "MazeMap TEXT NOT NULL, "
        "MovePath TEXT NOT NULL);";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errorMessage);
    if (exit != SQLITE_OK) {
        std::cerr << "Error Create Table " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }

    sqlite3_close(DB);
}

//保存游戏记录到数据库
void CMazeGameDlg::SaveGameRecord(const std::string& startTime, const std::string& mazeMap, const std::string& movePath)
{
    sqlite3* DB;
    char* errorMessage;

    int exit = sqlite3_open("MazeGame.db", &DB);
    if (exit) {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    std::string sql = "INSERT INTO GameRecords (StartTime, MazeMap, MovePath) VALUES ('" + startTime + "', '" + mazeMap + "', '" + movePath + "');";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errorMessage);
    if (exit != SQLITE_OK) {
        std::cerr << "Error Insert " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }

    sqlite3_close(DB);
}

//调用保存函数
void CMazeGameDlg::OnGameStart()
{
    auto startTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char buffer[26];
    ctime_s(buffer, sizeof(buffer), &startTime);
    std::string startTimeStr = buffer;

    std::string mazeMap = "Maze map data"; // 这里替换为实际的迷宫地图数据
    std::string movePath = "Move path data"; // 这里替换为实际的移动路径数据

    SaveGameRecord(startTimeStr, mazeMap, movePath);
}
