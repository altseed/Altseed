
#pragma once

#include <ace.common.Base.h>

#include <Math/ace.Vector2DI.h>
#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Graphics/ace.Color.h>

#include <thread>
#include <mutex>

#include "../../../ace.Core.Base.h"
#include "../../../ace.Core.Base_Imp.h"

namespace ace
{
	class Renderer3D
	{
	private:

		struct
		{
			std::set<RenderedObject3D*>	objects;
			std::set<RenderedObject3D*>	cameraObjects;
			std::set<RenderedObject3D*> directionalLightObjects;
		} rendering;

		Graphics_Imp*	m_graphics;

		std::set<RenderedObject3D*>	m_objects;
		std::set<RenderedObject3D*>	m_cameraObjects;
		std::set<RenderedObject3D*>	m_directionalLightObjects;

		std::thread				m_thread;
		volatile bool			m_running;
		volatile bool			m_drawOnce;

		bool					m_multithreadingMode;

		static void ThreadFunc(Renderer3D* renderer);
		void Rendering();

	public:
		Renderer3D(Graphics* graphics);
		~Renderer3D();

		void AddObject(RenderedObject3D* o);
		void RemoveObject(RenderedObject3D* o);

		void Flip();

		void BeginRendering();
		void EndRendering();
	};
}