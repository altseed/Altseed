
#pragma once

#include "asd.ImagePackage.h"

namespace asd
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

		std::vector<ImagePackageElementType>	elementTypes;
		std::vector<ImagePackageAdditionalElementType>	additionalElementTypes;

	public:

		static ImagePackage_Imp* Create(Graphics* graphics, const std::vector<uint8_t>& data);

		ImagePackage_Imp(Graphics* graphics);

		virtual ~ImagePackage_Imp();

		Texture2D* GetImage_(int32_t index) override;

		int32_t GetImageCount() override;

		const achar* GetImageName(int32_t index) override;

		RectI GetImageArea(int32_t index) override;

		ImagePackageElementType GetElementType(int32_t index);

		ImagePackageAdditionalElementType GetAdditionalElementType(int32_t index);

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() override { return ReferenceObject::GetRef(); }
		virtual int AddRef() override { return ReferenceObject::AddRef(); }
		virtual int Release() override { return ReferenceObject::Release(); }
#endif
	};
}