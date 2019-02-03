
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.RenderTexture2D_Imp.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
RenderTexture2D_Imp::RenderTexture2D_Imp(Graphics* graphics, ar::RenderTexture2D* rhi, Vector2DI size, TextureFormat format)
	: DeviceObject(graphics), m_size(size), rhi(rhi)
{
	m_type = TextureClassType::RenderTexture2D;
	m_format = format;
}

RenderTexture2D_Imp::~RenderTexture2D_Imp() { asd::SafeDelete(rhi); }

RenderTexture2D_Imp* RenderTexture2D_Imp::Create(Graphics* graphics, int32_t width, int32_t height, TextureFormat format)
{
	auto g = (Graphics_Imp*)graphics;

	auto rhi = ar::RenderTexture2D::Create(g->GetRHI());
	if (rhi->Initialize(g->GetRHI(), width, height, (ar::TextureFormat)format))
	{
		return new RenderTexture2D_Imp(g, rhi, Vector2DI(width, height), format);
	}

	asd::SafeDelete(rhi);
	return nullptr;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool RenderTexture2D_Imp::Save(const achar* path)
{
	std::vector<ar::Color> dst;
	int32_t width;
	int32_t height;

	if (rhi->Save(dst, width, height))
	{
		ar::ImageHelper::SavePNG(path, width, height, dst.data());
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace asd