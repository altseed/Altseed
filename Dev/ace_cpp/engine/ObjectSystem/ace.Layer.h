#pragma once
#include <memory>
#include <map>
#include "../ace.CoreToEngine.h"

namespace ace
{
	class Layer
	{
		friend class Scene;

	private:
		Scene* m_scene;
		virtual void SetScene(Scene* scene);

		virtual std::shared_ptr<CoreLayer> GetCoreLayer() const = 0;

		virtual void Update() = 0;
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

		virtual Scene* GetScene() const;
	};
}