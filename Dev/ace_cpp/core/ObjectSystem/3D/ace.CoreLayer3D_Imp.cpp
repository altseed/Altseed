
#include "ace.CoreLayer3D_Imp.h"
#include "../PostEffect/ace.CorePostEffect_Imp.h"
#include "../../Window/ace.Window_Imp.h"
#include "../../Graphics/ace.Graphics_Imp.h"

#include "ace.CoreObject3D.h"
#include "ace.CoreObject3D_Imp.h"

namespace ace
{
	CoreLayer3D_Imp::CoreLayer3D_Imp(Graphics* graphics, Log* log, Vector2DI windowSize, RenderSettings settings)
		: CoreLayer_Imp(graphics, windowSize)
		, m_objects(std::set<ObjectPtr>())
		, m_renderer(nullptr)
	{
		m_renderer = new Renderer3D(graphics, settings);
		m_renderer->SetWindowSize(windowSize);
	}

	CoreLayer3D_Imp::~CoreLayer3D_Imp()
	{
		for (auto& object : m_objects)
		{
			object->SetLayer(nullptr);
			object->Release();
		}
		m_objects.clear();

		SafeDelete(m_renderer);
	}

	RenderSettings CoreLayer3D_Imp::GetRenderSettings() const
	{
		return m_renderer->GetRenderSettings();
	}

	void CoreLayer3D_Imp::SetRenderSettings(RenderSettings settings)
	{
		m_renderer->SetRenderSettings(settings);
	}

	void CoreLayer3D_Imp::AddObject(ObjectPtr object3D)
	{
		if (m_objects.count(object3D) == 0)
		{
			m_objects.insert(object3D);
			object3D->SetLayer(this);
			SafeAddRef(object3D);
		}	
	}

	void CoreLayer3D_Imp::RemoveObject(ObjectPtr object3D)
	{
		if (m_objects.count(object3D) != 0)
		{
			m_objects.erase(object3D);
			object3D->SetLayer(nullptr);
			SafeRelease(object3D);
		}
	}

	void CoreLayer3D_Imp::SetSkyAmbientColor(Color color)
	{
		m_renderer->SetSkyAmbientColor(color);
	}

	void CoreLayer3D_Imp::SetGroundAmbientColor(Color color)
	{
		m_renderer->SetGroundAmbientColor(color);
	}

	void CoreLayer3D_Imp::BeginUpdating()
	{
		m_renderer->Flip();

		m_isDrawnTemp = m_isDrawn;

		if (m_isDrawnTemp)
		{
			m_renderer->BeginRendering();
		}
	}

	void CoreLayer3D_Imp::EndUpdating()
	{
		if (m_isDrawnTemp)
		{
			m_renderer->EndRendering();
		}
	}

	void CoreLayer3D_Imp::BeginDrawing()
	{
	}

	void CoreLayer3D_Imp::Draw()
	{
		if (!m_isDrawn)
		{
			return;
		}
	}

	void CoreLayer3D_Imp::EndDrawing()
	{
		m_layerRenderer->SetTexture(m_renderer->GetRenderTarget());

		m_triangles.clear();

		{
			ace::Vector2DF positions[4];
			ace::Color colors[4];
			ace::Vector2DF uvs[4];

			colors[0] = ace::Color(255, 255, 255, 255);
			colors[1] = ace::Color(255, 255, 255, 255);
			colors[2] = ace::Color(255, 255, 255, 255);

			positions[0].X = 0.0f;
			positions[0].Y = 0.0f;
			positions[1].X = 1.0f;
			positions[1].Y = 0.0f;
			positions[2].X = 1.0f;
			positions[2].Y = 1.0f;


			uvs[0].X = 0;
			uvs[0].Y = 0;
			uvs[1].X = 1;
			uvs[1].Y = 0;
			uvs[2].X = 1;
			uvs[2].Y = 1;

			m_layerRenderer->AddTriangle(positions, colors, uvs);
		}

		{
			ace::Vector2DF positions[4];
			ace::Color colors[4];
			ace::Vector2DF uvs[4];

			colors[0] = ace::Color(255, 255, 255, 255);
			colors[1] = ace::Color(255, 255, 255, 255);
			colors[2] = ace::Color(255, 255, 255, 255);

			positions[0].X = 0.0f;
			positions[0].Y = 1.0f;
			positions[1].X = 1.0f;
			positions[1].Y = 1.0f;
			positions[2].X = 0.0f;
			positions[2].Y = 0.0f;

			uvs[0].X = 0;
			uvs[0].Y = 1;
			uvs[1].X = 1;
			uvs[1].Y = 1;
			uvs[2].X = 0;
			uvs[2].Y = 0;

			m_layerRenderer->AddTriangle(positions, colors, uvs);
		}

		m_layerRenderer->DrawCache();
		m_layerRenderer->ClearCache();
	}
};