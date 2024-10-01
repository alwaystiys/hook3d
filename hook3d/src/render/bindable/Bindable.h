#pragma once
#include "base/Graphics.h"
#include "render/GraphicsResource.h"

class Bindable : public GraphicsResource
{
public:
	virtual void Bind(Graphics& gfx) noexcept = 0;
	virtual ~Bindable() = default;
};