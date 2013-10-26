#pragma once
#include <vector>
#include <memory>
#include "../ace.CoreToEngine.h"
#include "ace.Layer.h"
#include "2D/ace.Layer2D.h"

namespace ace
{
	class Engine;

	class Scene
	{
		friend class Engine;

	private:
		std::shared_ptr<CoreScene> m_coreScene;
		std::list<std::shared_ptr<Layer>> m_layers;

		void Update();
		void DrawAdditionally();
		void BeginDrawing();
		void EndDrawing();

	protected:
		virtual void OnUpdating();
		virtual void OnUpdated();

	public:
		Scene();
		virtual ~Scene();

		void AddLayer(const std::shared_ptr<Layer>& layer);
		void RemoveLayer(const std::shared_ptr<Layer>& layer);
	};
}
