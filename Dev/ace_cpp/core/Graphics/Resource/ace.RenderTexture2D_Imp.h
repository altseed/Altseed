
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

		eTextureFilterType	m_filter;

		RenderTexture2D_Imp(Graphics* graphics, Vector2DI size);
		virtual ~RenderTexture2D_Imp();
	public:

		Vector2DI GetSize() const { return m_size; }

		eTextureFilterType GetFilter() const { return m_filter; }
		void SetFilter(eTextureFilterType filter) { m_filter = filter; }


	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }

	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}