#include "InputLayout.h"

InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode)
{
	HRESULT hr;

	//D3D11_APPEND_ALIGNED_ELEMENT
	hr = GetDevice(gfx)->CreateInputLayout(
		layout.data(),
		(UINT)layout.size(),
		//(UINT)std::size(layout),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&pInputLayout
	);
}

void InputLayout::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}
