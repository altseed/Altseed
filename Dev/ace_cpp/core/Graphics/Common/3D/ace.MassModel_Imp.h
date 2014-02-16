
#pragma once

#include "ace.MassModel.h"

#include <Graphics/3D/ace.MassModel_IO.h>

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

		bool Load(Graphics_Imp* g, MassModel_IO& io);

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