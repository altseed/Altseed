#pragma once
#include <memory>
#include <map>
#include "../ace.CoreToEngine.h"
#include "Component\ace.LayerComponent.h"

namespace ace
{
	class Layer
	{
		friend class Scene;

	public:
		typedef std::shared_ptr<LayerComponent> ComponentPtr;

	private:
		std::map<astring, ComponentPtr> m_components;
		Scene* m_scene;

		virtual std::shared_ptr<CoreLayer> GetCoreLayer() const = 0;
		virtual void SetScene(Scene* scene);

		virtual void Update();
		virtual void UpdateObjects() = 0;
		virtual void DrawAdditionally() = 0;

		virtual void BeginDrawing() = 0;
		virtual void EndDrawing() = 0;

	protected:
		virtual void OnUpdated() = 0;
		virtual void OnUpdating() = 0;

	public:
		Layer();
		virtual ~Layer()
		{
		}

		virtual int GetDrawingPriority() const = 0;
		virtual void SetDrawingPriority(int value) = 0;

		void AddComponent(const ComponentPtr& component, astring key);
		ComponentPtr& GetComponent(astring key);
		void RemoveComponent(astring key);

		virtual Scene* GetScene() const;
	};
}