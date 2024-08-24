#pragma once
#include "utils/SdkConfig.h"
#include "utils/WrhConfig.h"
#include <d3d11.h>

using Microsoft::WRL::ComPtr;

class Graphics
{
public:
	Graphics(HWND hWnd, int width, int height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	void EndFrame();
	void ClearBuffer(float r, float g, float b) noexcept
	{
		const float color[] = { r, g, b, 1.0f };
		pContext->ClearRenderTargetView(pTarget.Get(), color);

	}
private:
	ComPtr<ID3D11Device> pDevice;
	ComPtr<IDXGISwapChain> pSwap;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<ID3D11RenderTargetView> pTarget;
	//ID3D11Device* pDevice = nullptr;
	//IDXGISwapChain* pSwap = nullptr;
	//ID3D11DeviceContext* pContext = nullptr;
	//ID3D11RenderTargetView* pTarget = nullptr;

	int width;
	int height;
};