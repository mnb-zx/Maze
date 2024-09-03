// CFailure.cpp: 实现文件
//

#include "pch.h"
#include "MazeGame.h"
#include "afxdialogex.h"
#include "CFailure.h"


// CFailure 对话框

IMPLEMENT_DYNAMIC(CFailure, CDialogEx)

CFailure::CFailure(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CFailure::~CFailure()
{
}

void CFailure::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFailure, CDialogEx)
END_MESSAGE_MAP()


// CFailure 消息处理程序
