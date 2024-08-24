#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")

Graphics::Graphics(HWND hWnd, int width, int height) : width(width), height(height)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// create device and front/back buffers, and swap chain and rendering context
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0 | D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	);

	//if (FAILED(hr)) {
	//	return;
	//}

	// gain access to texture subresource in swap chain(back buffer)
	//ID3D11Resource* pBackBuff = nullptr;
	//pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuff));
	//hr = pDevice->CreateRenderTargetView(
	//	pBackBuff,
	//	nullptr,
	//	&pTarget
	//);
	//pBackBuff->Release();
	ComPtr<ID3D11Resource> pBackBuff;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuff);
	hr = pDevice->CreateRenderTargetView(
		pBackBuff.Get(),
		nullptr,
		&pTarget
	);

	//if (FAILED(hr)) {
	//	return;
	//}
}

//Graphics::~Graphics()
//{
	//if (pTarget != nullptr) {
	//	pTarget->Release();
	//}
	//if (pContext != nullptr) {
	//	pContext->Release();
	//}
	//if (pSwap != nullptr) {
	//	pSwap->Release();
	//}
	//if (pDevice != nullptr) {
	//	pDevice->Release();
	//}
//}

void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u);
}
