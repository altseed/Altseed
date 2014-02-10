
#pragma once

#include "ace.MassModel.h"

namespace ace
{
	class MassObject_Imp
		: public MassObject
		, public ReferenceObject
	{
	private:
		std::shared_ptr<VertexBuffer_Imp>	m_vertexBuffer;
		std::shared_ptr<IndexBuffer_Imp>	m_indexBuffer;

		std::shared_ptr<Texture2D_Imp>		m_animationTexture;

	public:
		MassObject_Imp();
			virtual ~MassObject_Imp();

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}