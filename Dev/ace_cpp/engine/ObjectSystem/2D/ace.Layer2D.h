#pragma once
#include <memory>
#include <list>
#include <map>
#include "ace.Object2D.h"
#include "../../ace.CoreToEngine.h"
#include "../ace.Layer.h"
#include "../Component/ace.Layer2DComponent.h"

namespace ace
{
	class Layer2D : public Layer
	{
		friend class Scene;

	public:
		typedef std::shared_ptr<Object2D> ObjectPtr;
		typedef std::shared_ptr<Layer2DComponent> ComponentPtr;

	private:
		std::shared_ptr<CoreLayer2D>	m_coreLayer;
		std::list<ObjectPtr>			m_objects;
		std::map<astring, ComponentPtr> m_components;
		
		void BeginUpdateting();
		void EndUpdateting();

		void DrawAdditionally();
		void Update();

	protected:
		virtual void OnUpdating();
		virtual void OnUpdated();
		virtual void OnDrawAdditionally();

	public:
		Layer2D();
		virtual ~Layer2D();

		void AddObject(const ObjectPtr& object);
		void RemoveObject(const ObjectPtr& object);

		void AddComponent(const ComponentPtr& component, astring key);
		ComponentPtr& GetComponent(astring key);
		void RemoveComponent(astring key);
	};
}