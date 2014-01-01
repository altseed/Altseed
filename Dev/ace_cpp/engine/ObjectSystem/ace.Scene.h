#pragma once
#include <vector>
#include <memory>
#include "../ace.CoreToEngine.h"
#include "ace.Layer.h"
#include "2D/ace.Layer2D.h"
#include "Component/ace.SceneComponent.h"

namespace ace
{
	class Engine;

	class Scene
	{
		friend class Engine;

	public:
		typedef std::shared_ptr<Layer> LayerPtr;
		typedef std::shared_ptr<SceneComponent> ComponentPtr;

	private:
		std::shared_ptr<CoreScene> m_coreScene;
		std::list<LayerPtr> m_layersToDraw;
		std::list<LayerPtr> m_layersToUpdate;
		std::map<astring, ComponentPtr> m_components;

		void DrawAdditionally();
		void BeginDrawing();
		void EndDrawing();
		void Update();
		void UpdateComponents();

	protected:
		virtual void OnUpdating();
		virtual void OnUpdated();

	public:
		Scene();
		virtual ~Scene();

		void AddLayer(const LayerPtr& layer);
		void RemoveLayer(const LayerPtr& layer);

		void AddComponent(const ComponentPtr& component, astring key);
		ComponentPtr& GetComponent(astring key);
		void RemoveComponent(astring key);
	};
}
