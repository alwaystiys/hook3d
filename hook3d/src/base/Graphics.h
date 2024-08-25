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
	void ClearBuffer(float r, float g, float b) noexcept;
	void DrawTestTriangle();
private:
	ComPtr<ID3D11Device> pDevice;
	ComPtr<IDXGISwapChain> pSwap;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<ID3D11RenderTargetView> pTarget;

	int width;
	int height;
};