#include "pch.h"
#include "CChessManager.h"
#include <afxwin.h>
CChessManager::CChessManager() {
}
CChessManager::~CChessManager() {
}
bool CChessManager::Xy2Xy(int x0, int y0, int& x1, int& y1) {
	int x, y;
	for (int i=0;i<15;i++)
		for (int j = 0; j < 15; j++) {
			x = CChess::m_dx + i * CChess::m_d + CChess::m_d * 0.5;
			y = CChess::m_dy + j * CChess::m_d + CChess::m_d * 0.5;
			if (sqrt((x - x0) * (x - x0) + (y - y0) * (y - y0)) < 15) {
				x1 = i, y1 = j;
				return true;
			}
		}
	return false;
}
int CChessManager::Add(int x, int y) {
	int x1, y1;
	if (!Xy2Xy(x, y, x1, y1))
		return 1;
	return Add_Org(x1, y1);
}

int CChessManager::Add_Org(int x, int y) {
	int x1 = x, y1 = y;
	char tmp[20];
	if (enableLog) {
		_itoa_s(x1, tmp, 10);
		gameLog += tmp;
		gameLog += " ";
		_itoa_s(y1, tmp, 10);
		gameLog += tmp;
		gameLog += " \n";
	}

	for (int i = 0; i < m_nChess; i++)
		if (x1 == m_aChess[i].GetX() && y1 == m_aChess[i].GetY())
			return 2;
	m_aChess[m_nChess].Set(m_nChess, x1, y1, m_Color);
	m_nChess++;
	m_Color = (m_Color == WHITE ? BLACK : WHITE);
	return 0;
}

void CChessManager::Show(CDC* pDC) {
	for (int i = 0; i < m_nChess; i++)
		m_aChess[i].Show(pDC);
}
bool CChessManager::GameOver() {
	if (CheckRows())
		return true;
	if (CheckCols())
		return true;
	if (CheckLSlash())
		return true;
	if (CheckRSlash())
		return true;
	return false;
}
CChess* CChessManager::GetQz(int x, int y) {
	for (int i = 0; i < m_nChess; i++)
		if (m_aChess[i].GetX() == x && m_aChess[i].GetY() == y)
			return &m_aChess[i];
	return nullptr;
}
bool CChessManager::CheckRows() {
	CChess* pQz;
	COLOR color;
	int iCount;
	for (int i=0;i<MAX_ROWS;i++)
	{
		iCount = 0;
		for (int j = 0; j < MAX_COLS; j++)
			if (pQz = GetQz(j, i)) {
				if (iCount == 0) {
					color = pQz->GetColor();
					iCount++;
				}
				else if (color == pQz->GetColor()) {
					iCount++;
					if (iCount == WIN_NUM)
						return true;
				}
				else {
					color = pQz->GetColor();
					iCount = 1;
				}
			}
			else
				iCount = 0;
	}
	return false;
}
bool CChessManager::CheckCols() {
	CChess* pQz;
	COLOR color;
	int iCount;
	for (int i = 0; i < MAX_COLS; i++){
		iCount = 0;
		for (int j = 0; j < MAX_ROWS; j++)
			if (pQz = GetQz(i, j)) {
				if (iCount == 0) {
					color = pQz->GetColor();
					iCount++;
				}
				else if (color == pQz->GetColor()) {
					iCount++;
					if (iCount == WIN_NUM)
						return true;
				}
				else {
					color = pQz->GetColor();
					iCount = 1;
				}
			}
			else
				iCount = 0;
	}
	return false;
}
bool CChessManager::CheckLSlash() {
	CChess* pQz;
	COLOR color;
	int iCount;
	for (int i = -14; i < MAX_COLS; i++) {
		iCount = 0;
		for (int j = 0; j < MAX_ROWS; j++)
			if (pQz = GetQz(i+j, j)) {
				if (iCount == 0) {
					color = pQz->GetColor();
					iCount++;
				}
				else if (color == pQz->GetColor()) {
					iCount++;
					if (iCount == WIN_NUM)
						return true;
				}
				else {
					color = pQz->GetColor();
					iCount = 1;
				}
			}
			else
				iCount = 0;
	}
	return false;
}
bool CChessManager::CheckRSlash() 
{
	CChess* pQz;
	COLOR color;
	int iCount;
	for (int i = 0; i < MAX_COLS+14; i++) {
		iCount = 0;
		for (int j = 0; j < MAX_ROWS; j++)
			if (pQz = GetQz(i - j, j)) {
				if (iCount == 0) {
					color = pQz->GetColor();
					iCount++;
				}
				else if (color == pQz->GetColor()) {
					iCount++;
					if (iCount == WIN_NUM)
						return true;
				}
				else {
					color = pQz->GetColor();
					iCount = 1;
				}
			}
			else
				iCount = 0;
	}
	return false;
}