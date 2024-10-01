#pragma once
#include "render/bindable/Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
	void Bind(Graphics& gfx) noexcept override;
	UINT GetCount() const;
private:
	UINT count;
	ComPtr<ID3D11Buffer> pIndexdBuffer;
};
