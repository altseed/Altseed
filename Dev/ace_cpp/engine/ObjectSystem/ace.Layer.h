#pragma once
#include "../ace.CoreToEngine.h"
#include <memory>

namespace ace
{
	class Scene;

	class Layer
	{
		friend class Scene;

	private:
		virtual std::shared_ptr<CoreLayer> GetCoreLayer() const = 0;
		virtual void SetScene(Scene* scene) = 0;

		virtual void Update() = 0;
		virtual void DrawAdditionally() = 0;

		virtual void BeginDrawing() = 0;
		virtual void EndDrawing() = 0;

	public:
		virtual ~Layer()
		{
		}

		virtual int GetDrawingPriority() const = 0;
		virtual void SetDrawingPriority(int value) = 0;

		virtual Scene* GetScene() const = 0;
	};
}