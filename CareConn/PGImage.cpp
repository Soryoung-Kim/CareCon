#include "pch.h"
#include "PGImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Gdiplus;

CPGImage::CPGImage(void) : m_pImage(NULL), m_hBuffer(NULL)
{
}


CPGImage::~CPGImage(void)
{
	Unload();
}

int CPGImage::Load(LPCTSTR szName, LPCTSTR szType, HMODULE hInstance /*= NULL*/)
{
	Unload();

	HINSTANCE hInst = (NULL == hInstance) ? AfxGetInstanceHandle() : hInstance;
	HRSRC hRes = ::FindResource(hInst, szName, szType);
	if (NULL == hRes)
		return -1;

	DWORD dwImageSize = ::SizeofResource(hInst, hRes);
	if (0 == dwImageSize)
		return -2;

	HGLOBAL hResData = ::LoadResource(hInst, hRes);
	if (NULL == hResData)
		return -3;

	LPVOID pData = ::LockResource(hResData);
	if (NULL == pData)
		return -3;

	m_hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, dwImageSize);
	if (NULL == m_hBuffer)
		return -4;
	
	void* pBuffer = ::GlobalLock(m_hBuffer);
	if (NULL == pBuffer)
	{
		Unload();
		return -5;
	}

	::CopyMemory(pBuffer, pData, dwImageSize);
	::GlobalUnlock(m_hBuffer);

	CComPtr<IStream> pStream = NULL;
	HRESULT hr = ::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream);
	if (FAILED(hr) || pStream == NULL)
	{
		Unload();
		return -6;
	}

	m_pImage = Gdiplus::Image::FromStream(pStream);
	if (NULL == m_pImage)
	{
		Unload();
		return -7;
	}

	return 0;
}

int CPGImage::Load(const BYTE* const lpData, DWORD dwSize)
{
	Unload();

	m_hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, dwSize);
	if (NULL == m_hBuffer)
		return -1;

	void* pBuffer = ::GlobalLock(m_hBuffer);
	if (NULL == pBuffer)
	{
		Unload();
		return -1;
	}

	::CopyMemory(pBuffer, lpData, dwSize);
	::GlobalUnlock(m_hBuffer);

	CComPtr<IStream> pStream = NULL;
	HRESULT hr2 = ::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream);
	if (FAILED(hr2) || pStream == NULL)
	{
		Unload();
		return -2;
	}

	m_pImage = Gdiplus::Image::FromStream(pStream);
	if (NULL == m_pImage)
	{
		Unload();
		return -3;
	}

	return 0;
}

int CPGImage::Load(LPCTSTR szFileName)
{
	Unload();

	m_pImage = Gdiplus::Image::FromFile(szFileName);
	if (NULL == m_pImage)
	{
		Unload();
		return -1;
	}

	return 0;
}

void CPGImage::Unload()
{
	if (NULL != m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}

	if (NULL != m_hBuffer)
	{
		::GlobalUnlock(m_hBuffer);
		::GlobalFree(m_hBuffer);
		m_hBuffer = NULL;
	}
}

UINT CPGImage::GetWidth()
{
	if (NULL == m_pImage)
		return 0;

	return m_pImage->GetWidth();
}

UINT CPGImage::GetHeight()
{
	if (NULL == m_pImage)
		return 0;

	return m_pImage->GetHeight();
}

int CPGImage::RotateFlip(Gdiplus::RotateFlipType rotateFlipType)
{
	if (NULL == m_pImage)
		return -1;

	if (Gdiplus::Ok != m_pImage->RotateFlip(rotateFlipType))
		return -2;
	
	return 0;
}

Gdiplus::Image* CPGImage::operator&()
{
	return m_pImage;
}
