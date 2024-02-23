
// FiveInARowDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "NeoRenju.h"
#include "NeoRenjuDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <mmsystem.h>
#pragma comment(lib,"WINMM.LIB")
namespace fs = std::filesystem;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString readINI(CString section, CString key, CString def) {
	CString strStudName = def,path;
	path.Format(_T("%s%s"),fs::current_path().c_str(), _T("\\config.ini"));
	GetPrivateProfileStringW(section, key, def, strStudName.GetBuffer(MAX_PATH), MAX_PATH, path);
	return strStudName;
}

CString httpGet(CString URL) {
	std::string tmp(CW2A(URL.GetString()));
	tmp = "Bridge.exe -g " + tmp;
	WinExec(tmp.c_str(),SW_HIDE);
	std::ifstream fin("_res");
	std::string content((std::istreambuf_iterator<char>(fin)),
		(std::istreambuf_iterator<char>()));
	fin.close();
	CString ret(content.c_str());
	return ret;
}
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFiveInARowDlg 对话框



CFiveInARowDlg::CFiveInARowDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FIVEINAROW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFiveInARowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFiveInARowDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFiveInARowDlg 消息处理程序
 
BOOL CFiveInARowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	
	// IDM_ABOUTBOX 必须在系统命令范围内。
	
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetWindowPos(NULL, 0, 0, 1024, 768, SWP_NOZORDER | SWP_NOMOVE);
	m_FontTimer.CreatePointFont(250, _T("Segoe UI Semibold"), NULL);
	m_FontOver.CreatePointFont(1666, _T("微软雅黑"), NULL);
	m_bState = false;

	if (readINI(_T("Local"), _T("SaveLog"), _T("false")) == "true")
		m_Manager.EnableLog(true);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFiveInARowDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
		if ((nID & 0xFFF0) == SC_MOVE)
			PostMessage(WM_NCLBUTTONUP, nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFiveInARowDlg::OnPaint()
{
	if (readINI(_T("Local"), _T("Focus"), _T("false")) == "true")
		SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	CPaintDC dc(this);// 用于绘制的设备上下文
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);

	}

	CBitmap bmp;
	BITMAP bm;
	bmp.LoadBitmap(IDB_BITMAP_BK);
	bmp.GetObject(sizeof(BITMAP), &bm);
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap* pOldBitmap = MemDC.SelectObject(&bmp);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBitmap);
	m_Manager.Show(&dc);
	CDialog::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFiveInARowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFiveInARowDlg::OnLButtonDown(UINT nFlags, CPoint point) {
	if (NewGame(point.x, point.y))
		return;
	if (About(point.x, point.y))
		return;
	if (!m_bState) {
		AfxMessageBox(_T("请选择“开始”按钮开始新的游戏，按Esc键退出游戏！"));
		return;
	}
	
	if (readINI(_T("PVP"), _T("Enable"), _T("false")) == "true"){
		CString Prompt; Prompt.Format(L"%s%s%s", netURL, L"/up_to_date/", netPid);
		CString res = httpGet(Prompt);
		if (res == "false") {
			AfxMessageBox(L"请等待对方落子!");
			return;
		}

		int xx, yy;
		m_Manager.Xy2Xy(point.x, point.y, xx, yy);
		Prompt.Format(L"%s/stroke/%s/%d/%d", netURL,netPid,xx,yy);
		res = httpGet(Prompt);
		if (res == "false") {
			AfxMessageBox(L"网络错误！");
			return;
		}
	}

	int r = m_Manager.Add(point.x, point.y);
	if (r == 0) {
		CClientDC dc(this);
		m_Manager.Show(&dc);
		if (readINI(_T("Local"), _T("Sound"), _T("false")) == "true")
			PlaySound(MAKEINTRESOURCE(IDR_WAVE1),AfxGetResourceHandle(),SND_ASYNC | SND_RESOURCE | SND_NODEFAULT );
		if (m_Manager.GameOver()) {
			if (readINI(_T("Local"), _T("Record"), _T("false")) == "true") {
				std::ofstream fout("_stop");
				fout << "Stop.";
				fout.close();
			}
			if (readINI(_T("PVP"), _T("Enable"), _T("false")) != "true") {
				CString Prompt; Prompt.Format(L"%s/game_over", netURL);
				CString res = httpGet(Prompt);
			}

			if (readINI(_T("Local"), _T("SaveLog"), _T("false")) == "true") {
				std::ofstream fout(readINI(_T("Local"), _T("Logfile"), _T("replay.log")));
				fout << m_Manager.GetLog();
				fout.close();
			}
			KillTimer(1);
			KillTimer(2);
			CString csTemp;
			if (m_Manager.GetWinner() == WHITE)
				csTemp.Format(_T("白方胜!"));
			else
				csTemp.Format(_T("黑方胜！"));
			m_bState = false;
			CClientDC dc(this);
			CFont* pOldFont = dc.SelectObject(&m_FontOver);
			int OldBkMode = dc.GetBkMode();
			COLORREF OldColor, NewColor1 = RGB(60, 60, 60), NewColor2 = RGB(250, 50, 50);
			dc.SetBkMode(TRANSPARENT);
			OldColor = dc.SetTextColor(NewColor1);
			dc.TextOut(0, 0, csTemp);
			dc.SetTextColor(NewColor2);
			dc.TextOut(0, 0, csTemp);
			dc.SetTextColor(OldColor);
			dc.SetBkMode(OldBkMode);
			dc.SelectObject(pOldFont);
			if (readINI(_T("Local"), _T("Record"), _T("false")) == "true")
				AfxMessageBox(_T("录像文件已保存至'./out.mp4'。"));
		}
	}
	if (r == 1)
		AfxMessageBox(_T("请在棋盘交叉点落子！"));
	else if (r == 2)
		AfxMessageBox(_T("不可以重复落子！"));

	CDialogEx::OnLButtonDown(nFlags, point);
}
bool CFiveInARowDlg::NewGame(int x, int y) {
	int x0 = 35, y0 = 150, x1 = 200, y1 = 185;
	if ((x >= x0 && x <= x1) && (y >= y0 && y <= y1)) {
		//回放模式
		if (readINI(_T("Local"), _T("Replay"), _T("false")) == "true") {
			std::ifstream fin(readINI(_T("Local"), _T("Logfile"), _T("replay.log")));
			int xx = 0, yy = 0;
			m_Manager.NewGame();
			while (fin >> xx >> yy) {
				m_Manager.Add_Org(xx, yy);
				CClientDC dc(this);
				m_Manager.Show(&dc);
				if (readINI(_T("Local"), _T("Sound"), _T("false")) == "true")
					PlaySound(MAKEINTRESOURCE(IDR_WAVE1), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);
				Sleep(500);
			}
			fin.close();
			return true;
		}

		if (readINI(_T("PVP"), _T("Enable"), _T("false")) == "true") {
			netPid = readINI(_T("PVP"), _T("ID"), _T("1"));
			netURL = readINI(_T("PVP"), _T("URL"), _T("http://127.0.0.1:1234"));

			CString Prompt; Prompt.Format(L"%s%s%s", netURL, L"/up_to_date/", netPid);
			CString res = httpGet(Prompt);
			if (res == "false") {
				m_Manager.SetColor(WHITE);
			}
			else {
				m_Manager.SetColor(BLACK);
			}
		}
		if (readINI(_T("Local"), _T("Record"), _T("false")) == "true")
			ShellExecute(NULL, _T("open"), _T("Bridge.exe"), _T("-r"), NULL, SW_HIDE);

		m_Manager.NewGame();
		Invalidate();
		m_iTime = 0;
		SetTimer(1, 1000, NULL);
		SetTimer(2, 500, NULL);
		m_bState = true;
		return true;
	}
	return false;
}
bool CFiveInARowDlg::About(int x, int y) {
	int x0 = 35, y0 = 70, x1 = 200, y1 = 95;
	if ((x >= x0 && x <= x1) && (y >= y0 && y <= y1)) {
		CAboutDlg dlg;
		dlg.DoModal();
		return true;
	}
	return false;
}

void CFiveInARowDlg::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
	case 1: {
		if (m_iTime != 0 && readINI(_T("PVP"), _T("Enable"), _T("false")) == "true" && !m_Manager.myHand())
			break;
		CClientDC dc(this);
		CFont* pOldFont;
		pOldFont = dc.SelectObject(&m_FontTimer);
		m_iTime++;
		CString csTemp;
		csTemp.Format(_T("%d:%02d"), m_iTime/60,m_iTime%60);
		COLORREF OldColor, NewColor = RGB(150, 50, 50);
		OldColor = dc.SetTextColor(NewColor);
		dc.TextOutW(725, 0, csTemp);
		dc.SetTextColor(OldColor);
		dc.SelectObject(pOldFont);
		break;
		}

	case 2: {
		if (readINI(_T("PVP"), _T("Enable"), _T("false")) != "true")
			break;

		CString Prompt;
		char lastID[101] = { 0 };
		Prompt.Format(_T("%s%s"), netURL, _T("/get_last"));
		CString result = httpGet(Prompt);
		int cnt = -1, lastX = -1, lastY = -1;
		std::ifstream fin("_res");
		fin >> cnt >> lastID >> lastX >> lastY;
		fin.close();
		
		if (cnt != netLastCnt &&  strcmp(lastID,CW2A(netPid.GetString())) && (lastX > 0 && lastY > 0)) {
			netLastCnt = cnt;
			m_Manager.Add_Org(lastX, lastY);
			CClientDC dc(this);
			m_Manager.Show(&dc);
			if (readINI(_T("Local"), _T("Sound"), _T("false")) == "true")
				PlaySound(MAKEINTRESOURCE(IDR_WAVE1), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);
			
			if (m_Manager.GameOver()) {
				if (readINI(_T("Local"), _T("Record"), _T("false")) == "true") {
					std::ofstream fout("_stop");
					fout << "Stop.";
					fout.close();
				}

				CString Prompt; Prompt.Format(L"%s/game_over", netURL);
				CString res = httpGet(Prompt);

				KillTimer(1);
				KillTimer(2);

				if (readINI(_T("Local"), _T("SaveLog"), _T("false")) == "true") {
					std::ofstream fout(readINI(_T("Local"), _T("Logfile"), _T("replay.log")));
					fout << m_Manager.GetLog();
					fout.close();
				}

				CString csTemp;
				if (m_Manager.GetWinner() == WHITE)
					csTemp.Format(_T("白方胜!"));
				else
					csTemp.Format(_T("黑方胜！"));
				m_bState = false;
				CClientDC dc(this);
				CFont* pOldFont = dc.SelectObject(&m_FontOver);
				int OldBkMode = dc.GetBkMode();
				COLORREF OldColor, NewColor1 = RGB(60, 60, 60), NewColor2 = RGB(250, 50, 50);
				dc.SetBkMode(TRANSPARENT);
				OldColor = dc.SetTextColor(NewColor1);
				dc.TextOut(0, 0, csTemp);
				dc.SetTextColor(NewColor2);
				dc.TextOut(0, 0, csTemp);
				dc.SetTextColor(OldColor);
				dc.SetBkMode(OldBkMode);
				dc.SelectObject(pOldFont);
				if (readINI(_T("Local"), _T("Record"), _T("false")) == "true")
					AfxMessageBox(_T("录像文件已保存至'./out.mp4'。"));
			}
		}
		break;
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}