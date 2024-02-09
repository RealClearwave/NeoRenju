#include "pch.h"
#include <afxwin.h>
#include "CChess.h"
#include "Resource.h"
int CChess::m_dx = 271;
int CChess::m_dy = 68;
double CChess::m_d = 36.5;
CChess::CChess() {
}
CChess::~CChess(){
}
void CChess::Set(int num, int x, int y, COLOR color) {
	m_Num = num;
	m_x = x;
	m_y = y;
	m_Color = color;
}
void CChess::Show(CDC* pDC) {
	CBitmap bmpMask;
	bmpMask.LoadBitmap(IDB_BITMAP_MASK);
	CBitmap bmpQz;
	if (m_Color == WHITE)
		bmpQz.LoadBitmap(IDB_BITMAP_BZ);
	else
		bmpQz.LoadBitmap(IDB_BITMAP_HZ);
	BITMAP bm;
	bmpQz.GetObject(sizeof(BITMAP), &bm);
	CDC mDc0;
	mDc0.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmapMask = mDc0.SelectObject(&bmpMask);
	CDC mDc1;
	mDc1.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmapQz = mDc1.SelectObject(&bmpQz);

	pDC->BitBlt(m_dx + m_x * m_d, m_dy + m_y * m_d, bm.bmWidth, bm.bmHeight, &mDc0, 0, 0, SRCAND);
	
	pDC->BitBlt(m_dx + m_x * m_d, m_dy + m_y * m_d, bm.bmWidth, bm.bmHeight, &mDc1, 0, 0, SRCPAINT);
	mDc1.SelectObject(pOldBitmapMask);
	mDc0.SelectObject(pOldBitmapQz);
}

