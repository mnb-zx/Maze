// MazeGameDlg.h: 头文件
//

#pragma once
#include "MazeGame.h"
#include <chrono>

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
    DECLARE_MESSAGE_MAP()
};

// CMazeGameDlg 对话框
class CMazeGameDlg : public CDialogEx
{
    // 构造
public:
    CMazeGameDlg(CWnd* pParent = nullptr);    // 标准构造函数
    void InitializeMaze(int size);
    void updateWall();
    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MAZEGAME_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    afx_msg BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnPaint();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    HICON m_hIcon;
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnTimer(UINT_PTR nIDEvent); // 添加计时器消息处理函数
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    DECLARE_MESSAGE_MAP()

private:
    Maze* m_pMaze;
    int m_mazeSize;
    std::pair<int, int> m_playerPos;
    bool m_bNeedUpdateWalls;
    void MovePossible(int dx, int dy);
    CDC m_memDC;
    CBitmap m_memBitmap;
    std::pair<int, int> m_prevPlayerPos;
    bool m_isGameEnded;
    // 添加一个数组来保存每次游戏 GetTime 返回的值
    std::vector<int> m_timeRecords;


    // 计时器相关成员变量
    CTime m_startTime;
    CTime m_currentTime;
    CTimeSpan elapsed;
    CEdit m_timerEdit;
    void UpdateTimerDisplay();
    void EndGame(); // 新增函数声明
    CStatic m_passRequireStatic;
    bool m_isTimerAlert; // 计时器是否已经进入警告状态
    CFont m_alertFont; // 警告字体
    void SetTimerAlert(bool alert); // 设置计时器警告状态
    int GetTime() const;

    //暂停游戏相关成员变量
    bool m_isPaused; // 游戏是否暂停
    void PauseGame();
    void ResumeGame();
    CButton m_pauseButton; // 暂停按钮
    CStatic m_showPauseStatic; // 显示暂停状态的静态文本框
    CTime m_pauseTime; // 记录暂停时的时间
    CTimeSpan m_totalPausedTime; // 记录总的暂停时间

	// 回放相关成员变量
	std::vector<std::vector<int>> m_mazeData; // 存储迷宫数据
    std::vector<std::pair<int, int>> m_playerPath; // 存储玩家路径

public:
    afx_msg void OnBnClickedPauseButton();
    void SaveTimeRecordsToFile();
    void UpdateExitTime();

	// 回放相关成员函数
    void InitDatabase();
    void SaveGameRecord(const std::string& startTime, const std::string& mazeMap, const std::string& movePath);
    void OnGameStart();
    std::vector<std::vector<int>> GetMazeData() const;
    std::vector<std::pair<int, int>> GetPlayerPath() const;
    std::pair<int, int> GetMazeStartPos() const { return m_pMaze->getStart(); }
    std::pair<int, int> GetMazeEndPos() const { return m_pMaze->getEnd(); }
};
extern CMazeGameDlg* pMazeGameDlg;