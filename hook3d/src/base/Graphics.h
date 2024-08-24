#pragma once
#include "utils/SdkConfig.h"
#include <d3d11.h>


class Graphics
{

public:
	Graphics(HWND hWnd, int width, int height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void EndFrame();
	void ClearBuffer(float r, float g, float b) noexcept
	{
		const float color[] = { r, g, b, 1.0f };
		pContext->ClearRenderTargetView(pTarget, color);

	}
private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;

	int width;
	int height;
};