
#pragma once

#include "../../ace.CoreToEngine.h"
#include "../ace.Layer.h"
#include "ace.Object3D.h"

namespace ace
{
	class Layer3D
		: public Layer
	{
		friend class Scene;

	public:
		typedef std::shared_ptr<Object3D> ObjectPtr;
	
	private:

		std::shared_ptr<CoreLayer3D>	m_coreLayer;
		std::list<ObjectPtr>			m_objects;

		void DrawAdditionally();
		void Update();

		void BeginDrawing();
		void EndDrawing();

	protected:
		virtual void OnUpdating();
		virtual void OnUpdated();
		virtual void OnDrawAdditionally();

	public:
		Layer3D();
		virtual ~Layer3D();

		void AddObject(const ObjectPtr& object);
		void RemoveObject(const ObjectPtr& object);

	};
}