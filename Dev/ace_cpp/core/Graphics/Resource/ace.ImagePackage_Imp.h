
#pragma once

#include "ace.ImagePackage.h"

namespace ace
{
	class ImagePackage_Imp
		: public ImagePackage
		, public ReferenceObject
	{
	private:
		Graphics*				graphics = nullptr;
		std::vector<Texture2D*>	textures;
		std::vector<astring>	names;
		std::vector<RectI>		areas;

	public:

		static ImagePackage_Imp* Create(Graphics* graphics, const std::vector<uint8_t>& data);

		ImagePackage_Imp(
			Graphics* graphics,
			const std::vector<Texture2D*>&	textures,
			const std::vector<astring>&		names,
			const std::vector<RectI>&		areas);

		virtual ~ImagePackage_Imp();

		Texture2D* GetImage_(int32_t index) override;

		int32_t GetImageCount() override;

		const achar* GetImageName(int32_t index) override;

		RectI GetImageArea(int32_t index) override;

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() override { return ReferenceObject::GetRef(); }
		virtual int AddRef() override { return ReferenceObject::AddRef(); }
		virtual int Release() override { return ReferenceObject::Release(); }
#endif
	};
}