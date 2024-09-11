// Cwlc.cpp: 实现文件
//

#include "pch.h"
#include "MazeGame.h"
#include "afxdialogex.h"
#include "Cwlc.h"
#include "MazeGameDlg.h"
//#include "CLookbackSelectDlg.h"



// Cwlc 对话框

IMPLEMENT_DYNAMIC(Cwlc, CDialogEx)

Cwlc::Cwlc(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDC_WELCOME, pParent), m_pAboutDlg(nullptr)
{
}

Cwlc::~Cwlc()
{
    // 确保删除"关于"对话框
    if (m_pAboutDlg != nullptr)
    {
        delete m_pAboutDlg;
        m_pAboutDlg = nullptr;
    }
}

void Cwlc::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cwlc, CDialogEx)
    ON_BN_CLICKED(IDC_SELECT_32, &Cwlc::OnBnClickedSelect32)
    ON_BN_CLICKED(IDC_SELECT_64, &Cwlc::OnBnClickedSelect64)
    ON_BN_CLICKED(IDC_EXIT, &Cwlc::OnBnClickedExit)
    ON_BN_CLICKED(IDC_ABOUT, &Cwlc::OnBnClickedAbout)
    //ON_BN_CLICKED(IDC_LOOKBACK_BUTTON, &Cwlc::OnBnClickedLookbackButton) 
END_MESSAGE_MAP()

void Cwlc::OnBnClickedSelect32()
{
    CMazeGameDlg dlg;
    dlg.InitializeMaze(32);
    this->ShowWindow(SW_HIDE);  // 隐藏欢迎对话框
    dlg.DoModal();
    this->ShowWindow(SW_SHOW); 
    EndDialog(IDOK); // 关闭对话框// 游戏对话框关闭后重新显示欢迎对话框
}

void Cwlc::OnBnClickedSelect64()
{
    CMazeGameDlg dlg;
    dlg.InitializeMaze(64);
    this->ShowWindow(SW_HIDE);  // 隐藏欢迎对话框
    dlg.DoModal();
    this->ShowWindow(SW_SHOW); 
    EndDialog(IDOK); // 关闭对话框// 游戏对话框关闭后重新显示欢迎对话框
}

void Cwlc::OnBnClickedExit()
{
    // TODO: 在此添加控件通知处理程序代码
    exit(0);
}


void Cwlc::OnBnClickedAbout()
{
    // TODO: 在此添加控件通知处理程序代码
    // 如果"关于"对话框还没有创建，就创建它
    if (m_pAboutDlg == nullptr)
    {
        m_pAboutDlg = new CAboutDlg();
        m_pAboutDlg->Create(IDD_ABOUTBOX, this);
    }

    // 显示"关于"对话框
    if (m_pAboutDlg != nullptr)
    {
        m_pAboutDlg->ShowWindow(SW_SHOW);
    }
}
//void Cwlc::OnBnClickedLookbackButton()
//{
//    CLookbackSelectDlg dlg;
//    dlg.DoModal();
//}

