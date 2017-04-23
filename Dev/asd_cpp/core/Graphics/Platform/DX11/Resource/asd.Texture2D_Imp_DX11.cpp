
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Texture2D_Imp_DX11.h"
#include "../asd.Graphics_Imp_DX11.h"

#include "../../../../Log/asd.Log_Imp.h"

#include "../../../../3rdParty/DirectXToolKit/DDSTextureLoader.h"

#include <sstream>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D_Imp_DX11::Texture2D_Imp_DX11(Graphics* graphics)
		: Texture2D_Imp(graphics)
	{
	
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D_Imp_DX11::Texture2D_Imp_DX11(Graphics* graphics, ar::Texture2D* rhi, Vector2DI size, TextureFormat format)
		: Texture2D_Imp(graphics)
		, rhi(rhi)
	{
		m_format = format;
		m_size = size;
		m_resource.resize(size.X * size.Y * ImageHelper::GetPitch(m_format));

		auto g = (Graphics_Imp*) GetGraphics();
		g->IncVRAM(ImageHelper::GetVRAMSize(GetFormat(), GetSize().X, GetSize().Y));
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D_Imp_DX11::~Texture2D_Imp_DX11()
	{
		auto g = (Graphics_Imp*) GetGraphics();
		g->DecVRAM(ImageHelper::GetVRAMSize(GetFormat(), GetSize().X, GetSize().Y));

		asd::SafeDelete(rhi);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D_Imp_DX11* Texture2D_Imp_DX11::Create(Graphics_Imp_DX11* graphics, uint8_t* data, int32_t size, bool isEditable, bool isSRGB)
	{
		if (size == 0) return nullptr;

		auto rhi = ar::Texture2D::Create(graphics->GetRHI());

		if (rhi->Initialize(graphics->GetRHI(), data, size, isEditable, isSRGB))
		{
			return new Texture2D_Imp_DX11(graphics, rhi, Vector2DI(rhi->GetWidth(), rhi->GetHeight()), (asd::TextureFormat)rhi->GetFormat());
		}

		asd::SafeDelete(rhi);

		return nullptr;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D_Imp_DX11* Texture2D_Imp_DX11::Create(Graphics_Imp_DX11* graphics, int32_t width, int32_t height, TextureFormat format, void* data)
	{
		auto rhi = ar::Texture2D::Create(graphics->GetRHI());

		if (rhi->Initialize(graphics->GetRHI(), width, height, (ar::TextureFormat)format, data, true))
		{
			return new Texture2D_Imp_DX11(graphics, rhi, Vector2DI(rhi->GetWidth(), rhi->GetHeight()), (asd::TextureFormat)rhi->GetFormat());
		}

		asd::SafeDelete(rhi);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Texture2D_Imp_DX11::Save(const achar* path)
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
	bool Texture2D_Imp_DX11::Lock(TextureLockInfomation* info)
	{
		return rhi->Lock((ar::TextureLockInfomation*)info);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Texture2D_Imp_DX11::Unlock()
	{
		rhi->Unlock();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Texture2D_Imp_DX11::Reload(void* data, int32_t size)
	{
		auto g = (Graphics_Imp*) GetGraphics();
		g->DecVRAM(ImageHelper::GetVRAMSize(GetFormat(), GetSize().X, GetSize().Y));

		asd::SafeDelete(rhi);

		rhi = ar::Texture2D::Create(g->GetRHI());


		if (rhi->Initialize(g->GetRHI(), data, size, false, this->m_format == TextureFormat::R8G8B8A8_UNORM_SRGB))
		{
		}

		g->IncVRAM(ImageHelper::GetVRAMSize(GetFormat(), GetSize().X, GetSize().Y));
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}