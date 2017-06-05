
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.RenderTexture2D_Imp_DX11.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture2D_Imp_DX11::RenderTexture2D_Imp_DX11(Graphics* graphics, ar::RenderTexture2D* rhi, Vector2DI size, TextureFormat format)
		: RenderTexture2D_Imp(graphics, size)
		, rhi(rhi)
	{
		m_format = format;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture2D_Imp_DX11::~RenderTexture2D_Imp_DX11()
	{
		asd::SafeDelete(rhi);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture2D_Imp_DX11* RenderTexture2D_Imp_DX11::Create(Graphics* graphics, int32_t width, int32_t height, TextureFormat format)
	{
		auto g = (Graphics_Imp_DX11*) graphics;

		auto rhi = ar::RenderTexture2D::Create(g->GetRHI());
		if (rhi->Initialize(g->GetRHI(), width, height, (ar::TextureFormat)format))
		{
			return new RenderTexture2D_Imp_DX11(g, rhi, Vector2DI(width, height), format);
		}

		asd::SafeDelete(rhi);
		return nullptr;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool RenderTexture2D_Imp_DX11::Save(const achar* path)
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
}