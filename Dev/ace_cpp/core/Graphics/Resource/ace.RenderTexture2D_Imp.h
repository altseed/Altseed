
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <Math/ace.Vector2DI.h>
#include "../ace.DeviceObject.h"
#include "ace.RenderTexture2D.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
		@brief	描画先として使用できるテクスチャ
	*/
	class RenderTexture2D_Imp
		: public RenderTexture2D
		, public DeviceObject
	{
	protected:
		Vector2DI	m_size;
		TextureFormat		m_format;

		RenderTexture2D_Imp(Graphics* graphics, Vector2DI size);
		virtual ~RenderTexture2D_Imp();
	public:

		Vector2DI GetSize() const override { return m_size; }
		TextureFormat GetFormat() const override { return m_format; }

	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }

	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}