#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include <Math/ace.Vector2DI.h>

#include "ace.Texture.h"
#include "../ace.DeviceObject.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class DepthBuffer_Imp
		: public Texture
		, public DeviceObject
	{
	protected:
		Vector2DI	m_size;

		DepthBuffer_Imp(Graphics* graphics, Vector2DI size);
		virtual ~DepthBuffer_Imp();
	public:

		Vector2DI GetSize() const { return m_size; }

		/**
		@brief	テクスチャのクラスの種類を取得する。
		@return	種類
		*/
		virtual TextureClassType GetType() override { return TextureClassType::DepthBuffer; }

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