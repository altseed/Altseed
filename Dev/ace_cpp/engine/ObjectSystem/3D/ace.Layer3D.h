
#pragma once

#include "../../ace.CoreToEngine.h"
#include "../ace.Layer.h"

namespace ace
{
	class Layer3D
		: public Layer
	{
		friend class Scene;
	private:

		std::shared_ptr<CoreLayer3D>	m_coreLayer;


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
	};
}