#include <iostream>
#include <windows.h>
#include <D2d1.h>

#pragma comment(lib, "D2d1.lib")


HRESULT OnRender(ID2D1HwndRenderTarget *m_pRenderTarget,
	ID2D1SolidColorBrush* m_pLightSlateGrayBrush)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

		// 绘制背景坐标
		int width = static_cast<int>(rtSize.width);
		int height = static_cast<int>(rtSize.height);

		for (int x = 0; x < width; x += 10)
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
				D2D1::Point2F(static_cast<FLOAT>(x), height),
				m_pLightSlateGrayBrush,
				0.5f
			);
		} // X 坐标网格

		for (int y = 0; y < height; y += 10)
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
				D2D1::Point2F(width, static_cast<FLOAT>(y)),
				m_pLightSlateGrayBrush,
				0.5f
			);
		} // Y坐标网格

		hr = m_pRenderTarget->EndDraw();

	}

	return hr;
}



int main( int argc, int argv )
{
	::CoInitialize(NULL);

	// 获取桌面句柄
	HWND hwnd = GetDesktopWindow();
	hwnd = ::FindWindowEx(hwnd, NULL, L"Notepad", L"aa.txt - 记事本");
	hwnd = ::FindWindowEx(hwnd, NULL, L"Edit", L"");
	// hwnd = ::FindWindowEx(hwnd, NULL, L"SysListView32", L"FolderView");

	ID2D1Factory* m_pDirect2dFactory = NULL;
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	RECT rc ;
	GetWindowRect( hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top
	);
	ID2D1HwndRenderTarget* m_pRenderTarget = NULL;
	hr = m_pDirect2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, size),
		&m_pRenderTarget );

	ID2D1SolidColorBrush* m_pLightSlateGrayBrush = NULL;
	hr = m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::LightSlateGray),
		&m_pLightSlateGrayBrush
	);

	hr = OnRender(m_pRenderTarget,	 m_pLightSlateGrayBrush);

	::CoUninitialize();

	return 0;
}
