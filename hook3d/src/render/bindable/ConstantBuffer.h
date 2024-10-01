#pragma once
#include "Bindable.h"

template<class C>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(Graphics& gfx, const C& consts) {
		HRESULT hr;

		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;
		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		hr = GetDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	}

	void Update(Graphics& gfx, const C& consts) {
		D3D11_MAPPED_SUBRESOURCE msr;
		GetContext(gfx)->Map(
			pConstantBuffer.Get(),
			0u,
			D3D11_MAP_WRITE_DISCARD,
			0u,
			&msr
		);
		memcpy(msr.pData, &consts, sizeof(consts));
		GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
	}
protected:
	ComPtr<ID3D11Buffer> pConstantBuffer;
};

template<class C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
public:
	void Bind(Graphics& gfx) noexcept override {
		//this->GetContext(gfx)->VSSetConstantBuffers(0u, 1u, this->pConstantBuffer.GetAddressOf());
		::GetContext(gfx)->VSSetConstantBuffers(0u, 1u, ::pConstantBuffer.GetAddressOf());
	}
};

template<class C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
public:
	void Bind(Graphics& gfx) noexcept override {
		::pContext->PSSetConstantBuffers(0u, 1u, ::pConstantBuffer.GetAddressOf());
	}
};