
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../../Resource/asd.RenderTexture2D_Imp.h"
#include "../asd.Graphics_Imp_DX11.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class RenderTexture2D_Imp_DX11
		: public RenderTexture2D_Imp
	{
	protected:
		ar::RenderTexture2D*		rhi = nullptr;

		RenderTexture2D_Imp_DX11(Graphics* graphics, ar::RenderTexture2D* rhi, Vector2DI size, TextureFormat format);
		virtual ~RenderTexture2D_Imp_DX11();
	public:

		static RenderTexture2D_Imp_DX11* Create(Graphics* graphics, int32_t width, int32_t height, TextureFormat format);

		bool Save(const achar* path) override;

		bool Lock(TextureLockInfomation* info) override { return false; };

		void Unlock() override {};

		ar::RenderTexture2D* GetRHI() const { return rhi; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}