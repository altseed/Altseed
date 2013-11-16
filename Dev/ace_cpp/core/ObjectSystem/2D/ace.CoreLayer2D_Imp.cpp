
#include <list>
#include "ace.CoreLayer2D_Imp.h"
#include "../PostEffect/ace.CorePostEffect_Imp.h"
#include "../../Window/ace.Window_Imp.h"
#include "../../Graphics/Common/ace.Graphics_Imp.h"

using namespace std;

namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreLayer2D_Imp::CoreLayer2D_Imp(Graphics* graphics, Log* log, Vector2DI windowSize)
		: m_objects(list<ObjectPtr>())
		, m_graphics(nullptr)
		, m_renderer(nullptr)
		, m_layerRenderer(nullptr)
		, m_drawingPriority(0)
		, m_renderTarget0(nullptr)
		, m_renderTarget1(nullptr)
		, m_targetToLayer(-1)
		, m_layerSize(windowSize)
		, m_windowSize(windowSize)
	{
		m_renderer = new Renderer2D_Imp(graphics, log, windowSize);
		m_layerRenderer = new LayerRenderer(graphics);
		m_layerRenderer->SetWindowSize(windowSize);

		{
			ace::Vector2DF lpos[4];
			lpos[0].X = 0;
			lpos[0].Y = 0;
			lpos[1].X = windowSize.X;
			lpos[1].Y = 0;
			lpos[2].X = windowSize.X;
			lpos[2].Y = windowSize.Y;
			lpos[3].X = 0;
			lpos[3].Y = windowSize.Y;
			m_layerRenderer->SetLayerPosition(lpos);
		}

		m_graphics = (Graphics_Imp*) graphics;
		SafeAddRef(m_graphics);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreLayer2D_Imp::~CoreLayer2D_Imp()
	{
		ClearPostEffects();

		SafeRelease(m_renderTarget0);
		SafeRelease(m_renderTarget1);

		SafeDelete(m_renderer);
		SafeRelease(m_layerRenderer);

		for (auto& object : m_objects)
		{
			SafeRelease(object);
		}

		SafeRelease(m_graphics);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::CreateRenderTarget(const Vector2DI& size)
	{
		if (m_layerSize == size && m_renderTarget0 != nullptr) return;
		m_layerSize = size;

		SafeRelease(m_renderTarget0);
		SafeRelease(m_renderTarget1);

		m_renderTarget0 = m_graphics->CreateRenderTexture_Imp(m_layerSize.X, m_layerSize.Y, eTextureFormat::TEXTURE_FORMAT_RGBA8888);
		m_renderTarget1 = m_graphics->CreateRenderTexture_Imp(m_layerSize.X, m_layerSize.Y, eTextureFormat::TEXTURE_FORMAT_RGBA8888);
	}

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

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::AddPostEffect(CorePostEffect* postEffect)
	{
		SafeAddRef(postEffect);
		m_postEffects.push_back(postEffect);

		CreateRenderTarget(m_layerSize);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::ClearPostEffects()
	{
		for (auto& p : m_postEffects)
		{
			SafeRelease(p);
		}

		m_postEffects.clear();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture2D* CoreLayer2D_Imp::GetRenderTarget0()
	{
		return m_renderTarget0;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture2D* CoreLayer2D_Imp::GetRenderTarget1()
	{
		return m_renderTarget1;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::SetTargetToLayer(int32_t index)
	{
		m_targetToLayer = index;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Renderer2D* CoreLayer2D_Imp::GetRenderer() const
	{
		return m_renderer;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	int CoreLayer2D_Imp::GetDrawingPriority() const
	{
		return m_drawingPriority;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::SetDrawingPriority(int value)
	{
		m_drawingPriority = value;
	}

}