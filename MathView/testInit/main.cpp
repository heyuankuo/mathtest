#include <windows.h>
#include <D3D11.h>
#include <DirectXMath.h>
#include <assert.h>

#pragma comment(lib, "D3D11.lib")

const UINT width = 500, height = 500;

int main( int argc, char *argv[])
{

	// 获取桌面句柄
	HWND hwnd = GetDesktopWindow();
	hwnd = FindWindowEx(hwnd, NULL, L"Progman", L"Program Manager");
	hwnd = FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL);
	hwnd = FindWindowEx(hwnd, NULL, L"SysListView32", L"FolderView");

	// 创建设备及其上下文
	ID3D11Device *dev = NULL;
	D3D_FEATURE_LEVEL   FeatureLevel;
	ID3D11DeviceContext *pImmediateContext = NULL;
	HRESULT hr = D3D11CreateDevice(NULL,	// 默认设备
		D3D_DRIVER_TYPE_HARDWARE,			// 创建的设备类型（此处为硬件渲染）
		NULL,								// 软件渲染句柄
		D3D11_CREATE_DEVICE_DEBUG,			// 运行时层
		0,// 特效默认级别
		0, // 指定特效数量
		D3D11_SDK_VERSION,
		&dev,
		&FeatureLevel,
		&pImmediateContext );

	// 检测设备4X MSAA 级别
	UINT m4xMsaaQuality;
	dev->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_SNORM, 4, &m4xMsaaQuality);
	assert(m4xMsaaQuality > 0);

	// 创建交换链
	DXGI_SWAP_CHAIN_DESC dsc_desc;		// 配置交换链描述结构
	dsc_desc.BufferDesc.Width = width;
	dsc_desc.BufferDesc.Height = height;
	dsc_desc.BufferDesc.RefreshRate.Numerator = 60;
	dsc_desc.BufferDesc.RefreshRate.Denominator = 1;
	dsc_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dsc_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dsc_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	dsc_desc.SampleDesc.Count = 4;
	dsc_desc.SampleDesc.Quality = m4xMsaaQuality - 1;

	dsc_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dsc_desc.BufferCount = 1;
	dsc_desc.OutputWindow = hwnd;
	dsc_desc.Windowed = true;
	dsc_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	dsc_desc.Flags = 0;

	IDXGIDevice *dxgiDevice = 0;
	dev->QueryInterface(__uuidof(IDXGIDevice), (void **)&dxgiDevice);
	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&dxgiAdapter);
	IDXGIFactory *dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&dxgiFactory);

	IDXGISwapChain *mSwapChain = NULL;
	dxgiFactory->CreateSwapChain(dev, &dsc_desc, &mSwapChain); // 创建交换链

	dxgiFactory->Release();
	dxgiAdapter->Release();
	dxgiDevice->Release();

	// 创建呈现目标视图
	ID3D11RenderTargetView *mRtv;
	ID3D11Texture2D *backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&backBuffer));
	dev->CreateRenderTargetView(backBuffer, 0, &mRtv);
	backBuffer->Release();

	// 创建深度/模板缓冲区和相应视图（corresponding view）
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthStencilDesc.SampleDesc.Count = 4;
	depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	ID3D11Texture2D *mDepthStencilBuffer = NULL;
	ID3D11DepthStencilView *mDepthStencilView = NULL;
	dev->CreateTexture2D(&depthStencilDesc,	0,	&mDepthStencilBuffer);
	dev->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);

	// 将目标呈现视图和深度模板视图绑定到渲染管线
	pImmediateContext->OMSetRenderTargets(1, &mRtv, mDepthStencilView);

	// 设置视口
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0F;
	vp.Width = static_cast<float>(width);
	vp.Height = static_cast<float>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 0.0f;
	pImmediateContext->RSSetViewports(1, &vp);

	//设置为线框绘制模式
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(rsDesc));
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.DepthClipEnable = true;
	//D3D11_FILL_WIREFRAME以线框模式绘制，D3D11_FILL_SOLID是以实体模式绘制
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	rsDesc.FrontCounterClockwise = false;
	ID3D11RasterizerState *rsState(nullptr);
	dev->CreateRasterizerState(&rsDesc, &rsState);
	pImmediateContext->RSSetState(rsState);

	// 渲染一个绿色的窗口
	DirectX::XMVECTORF32 color = { 0.f, 1.f, 0.f, 1.6f };
	pImmediateContext->ClearRenderTargetView(mRtv, reinterpret_cast<float*>(&color));
	pImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	// 全部的场景绘制工作在这里面进行.....


	// 最后显示
	mSwapChain->Present(0, 0);

	// 回收资源
	dev->Release();
	dev = NULL;
	pImmediateContext->Release();
	pImmediateContext = NULL;

	return 0;
}