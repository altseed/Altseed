
#include "asd.DepthBuffer_Imp.h"
#include "../asd.Graphics_Imp.h"

namespace asd
{

DepthBuffer_Imp::DepthBuffer_Imp(Graphics* graphics, ar::DepthTexture* rhi, Vector2DI size) : DeviceObject(graphics), m_size(size), rhi(rhi)
{
	auto g = (Graphics_Imp*)GetGraphics();
	g->IncVRAM(m_size.X * m_size.Y * 4);
}

DepthBuffer_Imp::~DepthBuffer_Imp()
{
	auto g = (Graphics_Imp*)GetGraphics();
	g->DecVRAM(m_size.X * m_size.Y * 4);
	asd::SafeDelete(rhi);
}

DepthBuffer_Imp* DepthBuffer_Imp::Create(Graphics* graphics, int32_t width, int32_t height)
{
	auto g = (Graphics_Imp*)graphics;

	auto rhi = ar::DepthTexture::Create(g->GetRHI());
	if (rhi->Initialize(g->GetRHI(), width, height))
	{
		return new DepthBuffer_Imp(graphics, rhi, Vector2DI(width, height));
	}

	asd::SafeDelete(rhi);
	return nullptr;
}

} // namespace asd