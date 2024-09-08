#include "Graphics.h"
#include <d3dcompiler.h>
#include <array>
#include <iostream>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")


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
	//sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
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

	ComPtr<ID3D11Resource> pBackBuff;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuff);
	hr = pDevice->CreateRenderTargetView(
		pBackBuff.Get(),
		nullptr,
		&pTarget
	);

	// Create depth stencil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	ComPtr<ID3D11DepthStencilState> pDSSState;
	pDevice->CreateDepthStencilState(&dsDesc, &pDSSState);

	// Bind depth state
	pContext->OMSetDepthStencilState(pDSSState.Get(), 1u);


	// Create depth stencil texture
	ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	// Create view of depth stensil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	pDevice->CreateDepthStencilView(
		pDepthStencil.Get(), &descDSV, &pDSV
	);


	// Bind depth state
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

	



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


}


void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u);
}

void Graphics::ClearBuffer(float r, float g, float b) noexcept
{
	const float color[] = { r, g, b, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawVertexTriangle() {

	HRESULT hr;
	struct Vertex
	{
		struct
		{
			float x;
			float y;
		} pos;
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
		} color;
	};
	const Vertex vertices[] = {
		//{ 0.0f, 0.5f, 1.0f, 0.0f, 0.0f},
		//{ 0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
		//{ -0.5f, -0.5f, 0.0f, 0.0f, 1.0f},
		{ 0.0f, 0.5f, 255, 0, 0},
		{ 0.5f, -0.5f, 0, 255, 0},
		{ -0.5f, -0.5f, 0, 0, 255},
	};

	ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	hr = pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);

	// Bind vertex buffer to pipeplie
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// Create vertex shader
	ComPtr<ID3D11VertexShader> pVertexShader;
	ComPtr<ID3DBlob> pBlob;
	hr = D3DReadFileToBlob(L"shaderBins/VertexShader.cso", &pBlob);
	hr = pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

	// Bind vertex shader
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	// Input vertex layout
	ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	//D3D11_APPEND_ALIGNED_ELEMENT
	hr = pDevice->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);

	// Create pixel shader
	ComPtr<ID3D11PixelShader> pPixelShader;

	hr = D3DReadFileToBlob(L"shaderBins/PixelShader.cso", &pBlob);
	hr = pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	// Bind vertex shader
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	// bind vertex layout
	pContext->IASetInputLayout(pInputLayout.Get());

	// Bind render target
	//pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	// Set primitive topology to triangle list
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// viewport always fullscreen (for now)
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &vp);

	pContext->Draw((UINT)std::size(vertices), 0u);
}

void Graphics::DrawIndexPolygon(float angle, float x, float y)
{
	HRESULT hr;
	struct Vertex
	{
		struct
		{
			float x;
			float y;
		} pos;
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
	};
	const Vertex vertices[] = {
		//{ 0.0f, 0.5f, 1.0f, 0.0f, 0.0f},
		//{ 0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
		//{ -0.5f, -0.5f, 0.0f, 0.0f, 1.0f},
		{ 0.0f, 0.5f, 255, 0, 0, 0},
		{ 0.5f, -0.5f, 0, 255, 0, 0},
		{ -0.5f, -0.5f, 0, 0, 255, 0},
		{ -0.3f, 0.3f, 0, 255, 0, 0},
		{ 0.3f, 0.3f, 0, 0, 255, 0},
		{ 0.0f, -1.0f, 255, 0, 0, 0},
	};

	ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	hr = pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);

	// Bind vertex buffer to pipeplie
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// create index buffer
	const unsigned short indices[] = {
		0, 1, 2,
		0, 2, 3,
		0, 4, 1,
		2, 1, 5
	};

	ComPtr<ID3D11Buffer> pIndexdBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned int);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	hr = pDevice->CreateBuffer(&ibd, &isd, &pIndexdBuffer);

	// Bind index buffer to pipeplie
	pContext->IASetIndexBuffer(pIndexdBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// Create constant buffer for transformation matrix
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
		/*	struct {
				float element[4][4];
			} transformation;*/
	};

	const ConstantBuffer cb = {
		{
			/*((float)height / width) * std::cos(angle), std::sin(angle), 0.0f, 0.0f,
			((float)height / width) * -std::sin(angle), std::cos(angle), 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,*/
			/*		DirectX::XMMatrixMultiply(
						DirectX::XMMatrixRotationZ(angle), DirectX::XMMatrixScaling((float)height / width, 1.0f, 1.0f)
					)*/

					{
						DirectX::XMMatrixTranspose(
							DirectX::XMMatrixRotationZ(angle) *
							DirectX::XMMatrixScaling((float)height / width, 1.0f, 1.0f) *
							DirectX::XMMatrixTranslation(x, y, 0.0f)
						)
					}
				}
	};
	ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	hr = pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer);

	// Bind constant buffer to vertex shader
	pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	// Create vertex shader
	ComPtr<ID3D11VertexShader> pVertexShader;
	ComPtr<ID3DBlob> pBlob;
	hr = D3DReadFileToBlob(L"shaderBins/VertexShader.cso", &pBlob);
	hr = pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

	// Bind vertex shader
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	// Input vertex layout
	ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	//D3D11_APPEND_ALIGNED_ELEMENT
	hr = pDevice->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);

	// Create pixel shader
	ComPtr<ID3D11PixelShader> pPixelShader;

	hr = D3DReadFileToBlob(L"shaderBins/PixelShader.cso", &pBlob);
	hr = pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	// Bind vertex shader
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	// bind vertex layout
	pContext->IASetInputLayout(pInputLayout.Get());

	// Bind render target
	//pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	// Set primitive topology to triangle list
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// viewport always fullscreen (for now)
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &vp);

	pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);
}

void Graphics::Draw3DCube(float angle, float x, float y)
{
	HRESULT hr;
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
	};
	const Vertex vertices[] = {
		// 
		{ -1.0f, -1.0f, -1.0f, 255, 0, 0, 0},
		{ 1.0f, -1.0f, -1.0f, 0, 255, 0, 0},
		{ -1.0f, 1.0f, -1.0f, 0, 0, 255, 0},
		{ 1.0f, 1.0f, -1.0f, 255, 255, 0, 0},

		{ -1.0f, -1.0f, 1.0f, 255, 0, 255, 0},
		{ 1.0f, -1.0f, 1.0f, 0, 255, 255, 0},
		{ -1.0f, 1.0f, 1.0f, 0, 0, 0, 0},
		{ 1.0f, 1.0f, 1.0f, 255, 255, 255, 0},
	};

	ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	hr = pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);

	// Bind vertex buffer to pipeplie
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// create index buffer
	const unsigned short indices[] = {
		0, 2, 1, 2, 3, 1,  // front 
		1, 3, 5, 3, 7, 5,  // right
		0, 4, 2, 2, 4, 6,  // left
		2, 6, 3, 3, 6, 7,  // top
		0, 1, 4, 1, 5, 4,  // bottom
		4, 5, 7, 4, 7, 6   // back
	};

	ComPtr<ID3D11Buffer> pIndexdBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned int);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	hr = pDevice->CreateBuffer(&ibd, &isd, &pIndexdBuffer);

	// Bind index buffer to pipeplie
	pContext->IASetIndexBuffer(pIndexdBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// Create constant buffer for transformation matrix
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
		/*	struct {
				float element[4][4];
			} transformation;*/
	};

	const ConstantBuffer cb = {
		{
			{
				DirectX::XMMatrixTranspose(
					DirectX::XMMatrixRotationZ(angle) *
					DirectX::XMMatrixRotationX(angle)*
					//DirectX::XMMatrixScaling((float)height / width, 1.0f, 1.0f) *
					DirectX::XMMatrixTranslation(x, y, 4.0f) *
					DirectX::XMMatrixPerspectiveLH(1.0, 3.f / 4.f, 0.5f, 10.0f)
				)
			}
		}
	};
	ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	hr = pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer);

	// Bind constant buffer to vertex shader
	pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());


	// Create vertex shader
	ComPtr<ID3D11VertexShader> pVertexShader;
	ComPtr<ID3DBlob> pBlob;
	hr = D3DReadFileToBlob(L"shaderBins/CubeVertexShader.cso", &pBlob);
	hr = pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

	// Bind vertex shader
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	// Input vertex layout
	ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	//D3D11_APPEND_ALIGNED_ELEMENT
	hr = pDevice->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);

	// Create pixel shader
	ComPtr<ID3D11PixelShader> pPixelShader;

	hr = D3DReadFileToBlob(L"shaderBins/CubePixelShader.cso", &pBlob);
	hr = pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	// Bind vertex shader
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	// bind vertex layout
	pContext->IASetInputLayout(pInputLayout.Get());

	// Bind render target
	//pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	// Set primitive topology to triangle list
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// viewport always fullscreen (for now)
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &vp);

	pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);
}

void Graphics::Draw3DPureCube(float angle, float x, float z)
{
	HRESULT hr;
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;
	};
	const Vertex vertices[] = {
		// 
		{ -1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{ -1.0f, 1.0f, -1.0f},
		{ 1.0f, 1.0f, -1.0f},

		{ -1.0f, -1.0f, 1.0f},
		{ 1.0f, -1.0f, 1.0f},
		{ -1.0f, 1.0f, 1.0f},
		{ 1.0f, 1.0f, 1.0f},
	};

	ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	hr = pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);

	// Bind vertex buffer to pipeplie
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// create index buffer
	const unsigned short indices[] = {
		0, 2, 1, 2, 3, 1,  // front 
		1, 3, 5, 3, 7, 5,  // right
		0, 4, 2, 2, 4, 6,  // left
		2, 6, 3, 3, 6, 7,  // top
		0, 1, 4, 1, 5, 4,  // bottom
		4, 5, 7, 4, 7, 6   // back
	};

	ComPtr<ID3D11Buffer> pIndexdBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned int);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	hr = pDevice->CreateBuffer(&ibd, &isd, &pIndexdBuffer);

	// Bind index buffer to pipeplie
	pContext->IASetIndexBuffer(pIndexdBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// Create constant buffer for transformation matrix
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
		/*	struct {
				float element[4][4];
			} transformation;*/
	};

	const ConstantBuffer cb = {
		{
			{
				DirectX::XMMatrixTranspose(
					DirectX::XMMatrixRotationZ(angle) *
					DirectX::XMMatrixRotationX(angle) *
					DirectX::XMMatrixRotationY(angle)*
					//DirectX::XMMatrixScaling((float)height / width, 1.0f, 1.0f) *
					DirectX::XMMatrixTranslation(x, 0.0f, z + 4.0f) *
					DirectX::XMMatrixPerspectiveLH(1.0, 3.f / 4.f, 0.5f, 10.0f)
				)
			}
		}
	};
	ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	hr = pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer);

	// Bind constant buffer to vertex shader
	pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());


	struct ContantBuffer2
	{
		struct {
			float r;
			float b;
			float g;
			float a;
		} face_colors[6];
	};

	const ContantBuffer2 cb2 = {
		{
			{1.f, 0.f, 1.f, 1.f},
			{1.f, 0.f, 0.f, 1.f},
			{0.f, 1.f, 0.f, 1.f},
			{0.f, 0.f, 1.f, 1.f},
			{1.f, 1.f, 0.f, 1.f},
			{0.f, 1.f, 1.f, 1.f},
		}
	};

	ComPtr<ID3D11Buffer> pConstantBuffer2;
	D3D11_BUFFER_DESC cbd2 = {};
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DYNAMIC;
	cbd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(cb2);
	cbd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd2 = {};
	csd2.pSysMem = &cb2;
	hr = pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2);

	// Bind constant buffer to pixel shader
	pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());

	// Create vertex shader
	ComPtr<ID3D11VertexShader> pVertexShader;
	ComPtr<ID3DBlob> pBlob;
	hr = D3DReadFileToBlob(L"shaderBins/PureCubeVertexShader.cso", &pBlob);
	hr = pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

	// Bind vertex shader
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	// Input vertex layout
	ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	//D3D11_APPEND_ALIGNED_ELEMENT
	hr = pDevice->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);

	// Create pixel shader
	ComPtr<ID3D11PixelShader> pPixelShader;

	hr = D3DReadFileToBlob(L"shaderBins/PureCubePixelShader.cso", &pBlob);
	hr = pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	// Bind vertex shader
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	// bind vertex layout
	pContext->IASetInputLayout(pInputLayout.Get());

	// Bind render target
	//pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	// Set primitive topology to triangle list
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// viewport always fullscreen (for now)
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &vp);

	pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);
}
