// CImagingDlg.h
#pragma once
#include "afxdialogex.h"
#include <vector>
#include <utility>

// CImagingDlg 对话框
class CImagingDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CImagingDlg)

public:
    CImagingDlg(CWnd* pParent = nullptr);
    virtual ~CImagingDlg();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_IMAGING };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnPaint(); // 添加 OnPaint 函数
    DECLARE_MESSAGE_MAP()

public:
    std::vector<std::pair<int, int>> m_playerPath; // 存储玩家路径
    std::vector<std::vector<int>> m_mazeData; // 存储迷宫数据
    void LoadMazeData(const std::vector<std::vector<int>>& mazeData); // 添加加载迷宫数据的方法

private:
    size_t m_currentStep; // 当前回放的步骤
    std::pair<int, int> m_playerPos; // 当前玩家位置
    std::pair<int, int> m_startPos; // 迷宫起点
    std::pair<int, int> m_endPos; // 迷宫终点
    void LoadPlayerPath();
    void DrawMaze(CDC* pDC); // 添加绘制迷宫的函数
    void DrawPlayer(CDC* pDC); // 添加绘制玩家的函数
    void DrawStartAndEnd(CDC* pDC); // 添加绘制起点和终点的函数
};
