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
	void DrawVertexTriangle();
	void DrawIndexPolygon(float angle, float x, float y);
	void Draw3DCube(float angle, float x, float y);
	void Draw3DPureCube(float angle, float x, float z);
private:
	ComPtr<ID3D11Device> pDevice;
	ComPtr<IDXGISwapChain> pSwap;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<ID3D11RenderTargetView> pTarget;
	ComPtr<ID3D11DepthStencilView> pDSV;
	int width;
	int height;
};