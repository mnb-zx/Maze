// CEndGame.cpp: 实现文件
//

#include "pch.h"
#include "MazeGame.h"
#include "afxdialogex.h"
#include "CEndGame.h"


// CEndGame 对话框

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


BEGIN_MESSAGE_MAP(CEndGame, CDialogEx)
END_MESSAGE_MAP()


// CEndGame 消息处理程序
