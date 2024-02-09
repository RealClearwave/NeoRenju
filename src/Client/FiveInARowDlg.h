
// FiveInARowDlg.h: 头文件
//

#pragma once
#include "CChessManager.h"

// CFiveInARowDlg 对话框
class CFiveInARowDlg : public CDialogEx
{
// 构造
	CChessManager m_Manager;
	CFont m_FontTimer;
	CFont m_FontOver;
	int m_iTime;
	bool m_bState;
	int netLastCnt = -1;
	CString netPid, netURL; // 联机用的
public:
	CFiveInARowDlg(CWnd* pParent = nullptr);	// 标准构造函数
	
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIVEINAROW_DIALOG };
#endif

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	bool NewGame(int x, int y);
	bool About(int x, int y);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
