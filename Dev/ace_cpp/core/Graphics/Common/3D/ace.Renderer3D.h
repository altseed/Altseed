
#pragma once

#include <ace.common.Base.h>

#include <Math/ace.Vector2DI.h>
#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Graphics/ace.Color.h>

#include "../ace.RenderingThread.h"

#include "../../../ace.Core.Base.h"
#include "../../../ace.Core.Base_Imp.h"

namespace ace
{
	class Renderer3D
	{
	private:

		class RenderingEvent
			: public RenderingThreadEvent
		{
			Renderer3D*	m_renderer;
		public:
			RenderingEvent(Renderer3D* renderer);
			virtual ~RenderingEvent();
			void Event() override;
		};

		struct PasteConstantBuffer
		{
			float Size[4];
		};

		struct PasteVertex
		{
			Vector3DF	Position;
			Vector2DF	UV;
		};

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

		
		bool					m_multithreadingMode;

		Vector2DI				m_windowSize;

		RenderTexture_Imp*		m_renderTarget;

		std::shared_ptr<ace::VertexBuffer_Imp>	m_pasteVertexBuffer;
		std::shared_ptr<ace::IndexBuffer_Imp>	m_pasteIndexBuffer;
		std::shared_ptr<ace::NativeShader_Imp>	m_pasteShader;

		RenderingEvent	m_event;

		void Rendering();

	public:
		Renderer3D(Graphics* graphics);
		~Renderer3D();

		void SetWindowSize(Vector2DI windowSize);

		void AddObject(RenderedObject3D* o);
		void RemoveObject(RenderedObject3D* o);

		void Flip();

		void BeginRendering();
		void EndRendering();

		RenderTexture_Imp* GetRenderTarget();
	};
}