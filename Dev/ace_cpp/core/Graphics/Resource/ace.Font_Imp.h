#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <Graphics/Font/ace.AffLoader.h>
#include <Graphics/Font/ace.GlyphData.h>
#include <Math/ace.Vector2DI.h>

#include "ace.Font.h"
#include "../ace.DeviceObject.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Font_Imp
		: public Font
		, public DeviceObject
	{
	protected:

		std::map<achar, GlyphData> m_glyphs;

		Font_Imp(Graphics* graphics,std::map<achar,GlyphData> glyphs);
		virtual ~Font_Imp();

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}