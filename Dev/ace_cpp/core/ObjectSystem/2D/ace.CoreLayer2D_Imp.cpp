
#include <list>
#include "ace.CoreLayer2D_Imp.h"
#include "../../Window/ace.Window_Imp.h"
#include "../../Graphics/Common/ace.Graphics_Imp.h"
#include "../../Graphics/Common/2D/ace.LayerRenderer.h"

using namespace std;

namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreLayer2D_Imp::CoreLayer2D_Imp(Graphics* graphics, Log* log, Vector2DI windowSize)
		: CoreLayer_Imp(graphics, windowSize)
		, m_objects(list<ObjectPtr>())
		, m_renderer(nullptr)
	{
		m_renderer = new Renderer2D_Imp(graphics, log, windowSize);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreLayer2D_Imp::~CoreLayer2D_Imp()
	{
		SafeDelete(m_renderer);

		for (auto& object : m_objects)
		{
			SafeRelease(object);
		}

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::SetLayerShape(const Vector2DF& ul, const Vector2DF& ur, const Vector2DF& ll, const Vector2DF& lr, const Vector2DI& size)
	{
		ace::Vector2DF lpos[4];
		lpos[0] = ul;
		lpos[1] = ur;
		lpos[2] = lr;
		lpos[3] = ll;
		m_layerRenderer->SetLayerPosition(lpos);
	
		CreateRenderTarget(size);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::AddDrawnTriangle(
		const Vector2DF& pos1, const Vector2DF& uv1, const Color& col1,
		const Vector2DF& pos2, const Vector2DF& uv2, const Color& col2,
		const Vector2DF& pos3, const Vector2DF& uv3, const Color& col3)
	{
		Triangle t;
		t.Pos1 = pos1;
		t.UV1 = uv1;
		t.Col1 = col1;
		t.Pos2 = pos2;
		t.UV2 = uv2;
		t.Col2 = col2;
		t.Pos3 = pos3;
		t.UV3 = uv3;
		t.Col3 = col3;

		m_triangles.push_back(t);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::AddObject(ObjectPtr object)
	{
		m_objects.push_back(object);
		object->SetLayer(this);
		SafeAddRef(object);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::RemoveObject(ObjectPtr object)
	{
		m_objects.remove(object);
		object->SetLayer(nullptr);
		SafeRelease(object);
	}

	void CoreLayer2D_Imp::BeginUpdating()
	{

	}

	void CoreLayer2D_Imp::EndUpdating()
	{

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::BeginDrawing()
	{
		m_targetToLayer = -1;
		m_triangles.clear();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::Draw()
	{
		if (!m_isDrawn)
		{
			return;
		}

		for (auto& x : m_objects)
		{
			x->Draw();
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::EndDrawing()
	{
		if (m_postEffects.size() > 0)
		{
			m_graphics->SetRenderTarget(m_renderTarget0, nullptr);
		}

		m_renderer->DrawCache();
		m_renderer->ClearCache();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::EndDrawingAfterEffects()
	{
		if (m_postEffects.size() > 0)
		{
			m_graphics->SetRenderTarget(nullptr, nullptr);

			if (m_targetToLayer == 0)
			{
				m_layerRenderer->SetTexture(m_renderTarget0);
			}
			else if (m_targetToLayer == 1)
			{
				m_layerRenderer->SetTexture(m_renderTarget1);
			}

			if (m_triangles.size() > 0)
			{
				for (auto& t : m_triangles)
				{
					ace::Vector2DF positions[4];
					ace::Color colors[4];
					ace::Vector2DF uvs[4];

					colors[0] = t.Col1;
					colors[1] = t.Col2;
					colors[2] = t.Col3;

					positions[0] = t.Pos1;
					positions[1] = t.Pos2;
					positions[2] = t.Pos3;

					uvs[0] = t.UV1;
					uvs[1] = t.UV2;
					uvs[2] = t.UV3;

					m_layerRenderer->AddTriangle(positions, colors, uvs);
				}
			}
			else
			{
				{
					ace::Vector2DF positions[4];
					ace::Color colors[4];
					ace::Vector2DF uvs[4];

					colors[0] = ace::Color(255, 255, 255, 255);
					colors[1] = ace::Color(255, 255, 255, 255);
					colors[2] = ace::Color(255, 255, 255, 255);

					positions[0].X = -1.0f;
					positions[0].Y = -1.0f;
					positions[1].X = 1.0f;
					positions[1].Y = -1.0f;
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

					positions[0].X = -1.0f;
					positions[0].Y = 1.0f;
					positions[1].X = 1.0f;
					positions[1].Y = 1.0f;
					positions[2].X = -1.0f;
					positions[2].Y = -1.0f;

					uvs[0].X = 0;
					uvs[0].Y = 1;
					uvs[1].X = 1;
					uvs[1].Y = 1;
					uvs[2].X = 0;
					uvs[2].Y = 0;

					m_layerRenderer->AddTriangle(positions, colors, uvs);
				}
			}

			m_layerRenderer->DrawCache();
			m_layerRenderer->ClearCache();
		}
	}

	Renderer2D* CoreLayer2D_Imp::GetRenderer() const
	{
		return m_renderer;
	}

}