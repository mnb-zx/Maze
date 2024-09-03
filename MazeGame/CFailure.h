#pragma once
#include "afxdialogex.h"


// CFailure 对话框

class CFailure : public CDialogEx
{
	DECLARE_DYNAMIC(CFailure)

public:
	CFailure(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFailure();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
