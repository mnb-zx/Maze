#pragma once
#include "afxdialogex.h"


// CEndGame 对话框

class CEndGame : public CDialogEx
{
	DECLARE_DYNAMIC(CEndGame)

public:
	CEndGame(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEndGame();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENDGAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
