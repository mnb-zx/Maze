// CEndGame.cpp
#include "pch.h"
#include "MazeGame.h"
#include "afxdialogex.h"
#include "framework.h"
#include "CEndGame.h"
#include "CImagingDlg.h" 
#include "MazeGameDlg.h"


// CEndGame 对话框
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CEndGame, CDialogEx)
    ON_BN_CLICKED(IDC_EXITEND, &CEndGame::OnBnClickedExitEnd)
    ON_BN_CLICKED(IDC_HISTORY, &CEndGame::OnBnClickedHistory)
    ON_BN_CLICKED(IDC_PLAYBACK_BUTTON, &CEndGame::OnBnClickedPlaybackButton) // 添加按钮点击事件映射
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CEndGame, CDialogEx)

CEndGame::CEndGame(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_ENDGAME, pParent)
{
}

CEndGame::~CEndGame()
{
}

void CEndGame::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BOOL CEndGame::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // 其他初始化代码...
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEndGame::OnBnClickedExitEnd()
{
    EndDialog(IDOK); // 关闭对话框
}

void CEndGame::OnBnClickedHistory()
{
    ShellExecute(NULL, _T("open"), _T("TimeRecords.txt"), NULL, NULL, SW_SHOWNORMAL);
}

void CEndGame::OnBnClickedPlaybackButton()
{
    CImagingDlg imagingDlg;
    // 假设 pMazeGameDlg 是一个指向 MazeGameDlg 实例的指针
    extern CMazeGameDlg* pMazeGameDlg;
    imagingDlg.LoadMazeData(pMazeGameDlg->GetMazeData()); // 获取并加载迷宫数据
    imagingDlg.m_playerPath = pMazeGameDlg->GetPlayerPath(); // 获取玩家路径
    imagingDlg.DoModal(); // 弹出 IDD_IMAGING 窗口
}




