#pragma once

#include <Windows.h>
#include <GdiPlus.h>

#pragma comment(lib, "Gdiplus.lib")

class CPGImage
{
public:
	CPGImage();
	~CPGImage();

private:
	Gdiplus::Image* m_pImage;
	HGLOBAL m_hBuffer;

public:
	int Load(LPCTSTR szName, LPCTSTR szType, HMODULE hInstance = NULL);
	int Load(const BYTE* const lpData, DWORD dwSize);
	int Load(LPCTSTR szFileName);

	UINT GetWidth();
	UINT GetHeight();

	int RotateFlip(Gdiplus::RotateFlipType rotateFlipType);

	Gdiplus::Image* operator&();
private:
	void Unload();

};

