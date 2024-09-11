// CEndGame.h
#pragma once
#include "afxdialogex.h"
#include "afxwin.h"
class CImagingDlg; // 前向声明

// CEndGame 对话框
class CEndGame : public CDialogEx
{
    DECLARE_DYNAMIC(CEndGame)

public:
    CEndGame(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~CEndGame();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ENDGAME_DIALOG };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedExitEnd();
    afx_msg void OnBnClickedHistory();
    afx_msg void OnBnClickedPlaybackButton(); // 新增按钮点击事件声明
};
