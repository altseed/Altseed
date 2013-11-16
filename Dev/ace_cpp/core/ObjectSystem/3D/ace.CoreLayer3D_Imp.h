
#pragma once

#include "ace.CoreLayer3D.h"

#include "../ace.CoreLayer_Imp.h"
#include "../../Graphics/Common/3D/ace.Renderer3D.h"
#include "../../Graphics/Common/2D/ace.LayerRenderer.h"

#include <Math/ace.Vector2DI.h>

namespace ace
{
	class CoreLayer3D_Imp
		: public CoreLayer3D
		, public CoreLayer_Imp
		, public ReferenceObject
	{
	friend class ObjectSystemFactory_Imp;

	private:

		std::list<ObjectPtr> m_objects;

		Graphics_Imp*	m_graphics;

		Renderer3D*		m_renderer;
		LayerRenderer*	m_layerRenderer;

		int m_drawingPriority;

		Vector2DI		m_windowSize;

		CoreLayer3D_Imp(Graphics* graphics, Log* log, Vector2DI windowSize);
		virtual ~CoreLayer3D_Imp();

	public:

		void AddObject(ObjectPtr object3D);
		void RemoveObject(ObjectPtr object3D);

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