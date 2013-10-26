
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <Math/ace.Vector2DI.h>
#include "../ace.DeviceObject.h"
#include "ace.RenderTexture.h"

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
	class RenderTexture_Imp
		: public RenderTexture2D
		, public DeviceObject
	{
	protected:
		Vector2DI	m_size;

		RenderTexture_Imp(Graphics* graphics, Vector2DI size);
		virtual ~RenderTexture_Imp();
	public:

		Vector2DI GetSize() const { return m_size; }


	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }

	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}