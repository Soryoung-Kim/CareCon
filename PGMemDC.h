#pragma once

// Memory DC Class
class CPGMemDC : public CDC
{
private:
	CPGMemDC();
	CPGMemDC(const CPGMemDC& /*src*/);
	CPGMemDC& operator=(const CPGMemDC& /*src*/);

public:
	CPGMemDC(CWnd* pParent, UINT nIDResource = 0);
	virtual ~CPGMemDC();

	enum
	{
		ColorWhite = 0xFFFFFF, ColorRed = 0x0000FF, ColorGreen = 0x00FF00, ColorBlue = 0xFF0000,
		ColorPurple = 0xFF00FF, ColorYellow = 0x00FFFF, ColorCyan = 0xFFFF00, 
		ColorAliceBlue = 0xFFF8F0, ColorLavender = 0xFAE6E6, ColorAzure = 0xFFFFF0, ColorMintCream = 0xFAFFF5,
		ColorAquamarine = 0xD4FF7F, ColorMediumAquamarine = 0xAACD66, ColorPaleGreen = 0x98FB98,
		ColorGray = 0xBBBBBB
	};

protected:
	BOOL Attach(HDC hDC);
	HDC Detach();

private:
	CWnd*		m_pParent;
	PAINTSTRUCT	m_PaintStruct;
	CRect m_rcClient, m_rcWindow;

	CDC*		m_pTargetDC, m_MemoryDC;
	CBitmap		m_Bitmap, *m_pOldBitmap;
};

