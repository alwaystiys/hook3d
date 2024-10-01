#pragma once
#include "render/bindable/Bindable.h"

class VertexBuffer : public Bindable
{
public:
	template<class V>
	VertexBuffer(Graphics& gfx, const std::vector<V>& vertices);
	void Bind(Graphics& gfx) noexcept override;
private:
	UINT stride;
	ComPtr<ID3D11Buffer> pVertexBuffer;
};

