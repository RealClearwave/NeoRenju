#pragma once
#include <afxwin.h>
enum COLOR {BLACK, WHITE};
class CChess{
	int m_Num;
	int m_x;
	int m_y;
	COLOR m_Color;
public:
	CChess();
	~CChess();
	void Set(int num, int x, int y, COLOR color);
	COLOR GetColor() { return m_Color; }
	int GetX() { return m_x; }
	int GetY() { return m_y; }
	void Show(CDC* pDC);
	static int m_dx;
	static int m_dy;
	static double m_d;
};

