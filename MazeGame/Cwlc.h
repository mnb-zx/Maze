#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "MazeGameDlg.h"
#include "MazeGame.h"

// Cwlc 对话框

class Cwlc : public CDialogEx
{
    DECLARE_DYNAMIC(Cwlc)

public:
    Cwlc(CWnd* pParent = nullptr);
    virtual ~Cwlc();

    enum { IDD = IDC_WELCOME };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedSelect32();
    afx_msg void OnBnClickedSelect64();
    afx_msg void OnBnClickedExit();
    afx_msg void OnBnClickedAbout();
private:
    CAboutDlg* m_pAboutDlg;  // 指向"关于"对话框的指针
    // afx_msg void OnBnClickedLookbackButton();
};

