#pragma once
#include "afxdialogex.h"


// CWlcome 对话框

class CWlcome : public CDialogEx
{
	DECLARE_DYNAMIC(CWlcome)

public:
	CWlcome(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWlcome();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDC_WELCOME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
