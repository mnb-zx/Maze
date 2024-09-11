// CImagingDlg.cpp
#include "pch.h"
#include "MazeGame.h"
#include "MazeGameDlg.h"
#include "afxdialogex.h"
#include "CImagingDlg.h"
#include <fstream> 
#include <vector>
#include <utility>

// CImagingDlg 对话框
IMPLEMENT_DYNAMIC(CImagingDlg, CDialogEx)

CImagingDlg::CImagingDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_IMAGING, pParent), m_currentStep(0)
{
}

CImagingDlg::~CImagingDlg()
{
}

void CImagingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImagingDlg, CDialogEx)
    ON_WM_TIMER()
    ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CImagingDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    LoadPlayerPath(); // 加载玩家路径
    SetTimer(1, 200, NULL); // 设置定时器，每0.2秒触发一次

    // 获取 CMazeGameDlg 的起点和终点位置
    if (pMazeGameDlg != nullptr) {
        m_startPos = pMazeGameDlg->GetMazeStartPos();
        m_endPos = pMazeGameDlg->GetMazeEndPos();
    }

    return TRUE;  
}

void CImagingDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1)
    {
        if (m_currentStep < m_playerPath.size())
        {
            // 更新玩家位置
            m_playerPos = m_playerPath[m_currentStep];
            m_currentStep++;
            Invalidate(); // 触发重绘
        }
        else
        {
            KillTimer(1); // 停止定时器
        }
    }
    CDialogEx::OnTimer(nIDEvent);
}

void CImagingDlg::OnPaint()
{
    CPaintDC dc(this); // 用于绘制的设备上下文
    DrawMaze(&dc); // 绘制迷宫
    DrawPlayer(&dc); // 绘制玩家
    DrawStartAndEnd(&dc); // 绘制起点和终点
}

void CImagingDlg::DrawMaze(CDC* pDC)
{
    // 假设每个单元格的大小为20x20像素
    const int cellSize = 20;
    for (int y = 0; y < m_mazeData.size(); ++y)
    {
        for (int x = 0; x < m_mazeData[y].size(); ++x)
        {
            CRect cellRect(x * cellSize, y * cellSize, (x + 1) * cellSize, (y + 1) * cellSize);
            if (m_mazeData[y][x] == 1) // 墙
            {
                pDC->FillSolidRect(cellRect, RGB(0, 0, 0)); // 黑色
            }
            else // 路
            {
                pDC->FillSolidRect(cellRect, RGB(255, 255, 255)); // 白色
            }
        }
    }
}

void CImagingDlg::DrawPlayer(CDC* pDC)
{
    // 假设每个单元格的大小为20x20像素
    const int cellSize = 20;
    CRect playerRect(m_playerPos.first * cellSize, m_playerPos.second * cellSize,
        (m_playerPos.first + 1) * cellSize, (m_playerPos.second + 1) * cellSize);
    pDC->FillSolidRect(playerRect, RGB(0, 0, 255)); 
}

void CImagingDlg::DrawStartAndEnd(CDC* pDC)
{
    CBrush startBrush(RGB(0, 255, 0)); // 绿色
    CBrush endBrush(RGB(255, 0, 0)); // 红色

    // 绘制起点
    CRect startRect(m_startPos.first * 20, m_startPos.second * 20, (m_startPos.first + 1) * 20, (m_startPos.second + 1) * 20);
    pDC->FillRect(&startRect, &startBrush);

    // 绘制终点
    CRect endRect(m_endPos.first * 20, m_endPos.second * 20, (m_endPos.first + 1) * 20, (m_endPos.second + 1) * 20);
    pDC->FillRect(&endRect, &endBrush);
}

void CImagingDlg::LoadMazeData(const std::vector<std::vector<int>>& mazeData)
{
    m_mazeData = mazeData;
}

void CImagingDlg::LoadPlayerPath()
{
    // 从文件或其他地方加载玩家路径
    // 这里假设路径是从文件中加载的
    std::ifstream file("PlayerPath.txt");
    if (!file.is_open())
    {
        AfxMessageBox(_T("无法打开玩家路径文件"));
        return;
    }

    int x, y;
    while (file >> x >> y)
    {
        m_playerPath.emplace_back(x, y);
    }
}