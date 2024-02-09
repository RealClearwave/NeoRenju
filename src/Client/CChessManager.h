#pragma once
#include "CChess.h"
#define MAX_ROWS 15
#define MAX_COLS 15
#define MAX_CHESS MAX_ROWS*MAX_COLS
#define WIN_NUM 5
class CChessManager{
	CChess m_aChess[MAX_CHESS];
	int	m_nChess;
	COLOR m_Color,org_Color;
	bool CheckRows();
	bool CheckCols();
	bool CheckLSlash();
	bool CheckRSlash();
public:
	CChessManager();
	~CChessManager();
	void NewGame() { m_nChess = 0; m_Color = BLACK; }
	void SetColor(COLOR cc) { m_Color = cc; org_Color = cc; }
	bool myHand() { return m_Color == org_Color; }
	bool Xy2Xy(int x0, int y0, int& x1, int& y1);
	int Add(int x, int y);
	int Add_Org(int x, int y);
	void Show(CDC* pDC);
	bool GameOver();
	COLOR GetWinner() { return m_aChess[m_nChess - 1].GetColor(); }
	CChess* GetQz(int x, int y);
};

