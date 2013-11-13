
#pragma once

#include "ace.CoreLayer3D.h"

#include <Math/ace.Vector2DI.h>

namespace ace
{
	class CoreLayer3D_Imp
		: public CoreLayer3D
		, public ReferenceObject
	{
	friend class ObjectSystemFactory_Imp;

	private:

		int m_drawingPriority;

		CoreLayer3D_Imp(Graphics* graphics, Log* log, Vector2DI windowSize);
		virtual ~CoreLayer3D_Imp();

	public:

		void Draw();

		int GetDrawingPriority() const;
		void SetDrawingPriority(int value);

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}