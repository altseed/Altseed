
#include "ace.CoreLayer2D_Imp.h"
#include "../../Window/ace.Window_Imp.h"
#include "../../Graphics/ace.Graphics_Imp.h"
#include "../../Graphics/2D/ace.LayerRenderer.h"

#include "ace.CoreTextureObject2D_Imp.h"
#include "ace.CoreTextObject2D_Imp.h"
#include "ace.CoreCameraObject2D_Imp.h"
#include "ace.CoreMapObject2D_Imp.h"
#include "ace.CoreEffectObject2D_Imp.h"

#include "../../Core/ace.Core.h"

using namespace std;

namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreLayer2D_Imp::CoreLayer2D_Imp(Core* core, Graphics* graphics, Log* log, Vector2DI windowSize)
		: CoreLayer_Imp(graphics, windowSize)
		, core(core)
		, m_objects(list<ObjectPtr>())
		, m_cameras(list<CoreCameraObject2D*>())
		, m_renderer(nullptr)
		, m_rendererForCamera(nullptr)
	{
		m_renderer = new Renderer2D_Imp(graphics, log);
		m_rendererForCamera = new Renderer2D_Imp(graphics, log);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreLayer2D_Imp::~CoreLayer2D_Imp()
	{
		SafeDelete(m_renderer);
		SafeDelete(m_rendererForCamera);

		for (auto& object : m_objects)
		{
			object->SetLayer(nullptr);
			SafeRelease(object);
		}
		for (auto& camera : m_cameras)
		{
			camera->SetLayer(nullptr);
			SafeRelease(camera);
		}
	}

	CoreObject2D_Imp* CoreLayer2D_Imp::CoreObject2DToImp(ObjectPtr obj)
	{
		if (obj == nullptr) return nullptr;

		switch (obj->GetObjectType())
		{
		case Object2DType::Texture:
			return (CoreTextureObject2D_Imp*) obj;
		case Object2DType::Text:
			return (CoreTextObject2D_Imp*) obj;
		case Object2DType::Camera:
			return (CoreCameraObject2D_Imp*) obj;
		case Object2DType::Map:
			return (CoreMapObject2D_Imp*) obj;
		case Object2DType::Effect:
			return (CoreEffectObject2D_Imp*) obj;
		default:
			break;
		}

		return nullptr;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::AddObject(ObjectPtr object)
	{
		if (object->GetObjectType() == Object2DType::Camera)
		{
			auto camera = (CoreCameraObject2D_Imp*)object;
			m_cameras.push_back(camera);
			SafeAddRef(camera);
		}
		else
		{
			m_objects.push_back(object);
			SafeAddRef(object);
		}

		object->SetLayer(this);

		{
			auto o = CoreObject2DToImp(object);
			o->OnAdded(m_renderer);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::RemoveObject(ObjectPtr object)
	{
		{
			auto o = CoreObject2DToImp(object);
			o->OnRemoving(m_renderer);
		}

		object->SetLayer(nullptr);

		if (object->GetObjectType() == Object2DType::Camera)
		{
			auto camera = (CoreCameraObject2D*)object;
			m_cameras.remove(camera);
			SafeRelease(camera);
		}
		else
		{
			m_objects.remove(object);
			SafeRelease(object);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::BeginUpdating()
	{

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::EndUpdating()
	{
		m_renderer->GetEffectManager()->Update(core->GetDeltaTime());
		m_renderer->GetEffectManager()->Flip();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::BeginDrawing()
	{
		auto vanished = vector<ObjectPtr>();
		for (auto& x : m_objects)
		{
			if (!x->GetIsAlive())
			{
				vanished.push_back(x);
			}
		}

		for (auto& x : vanished)
		{
			RemoveObject(x);
		}
	}

	void CoreLayer2D_Imp::DrawObjects(Renderer2D* renderer)
	{
		for (auto& x : m_objects)
		{
			x->Draw(renderer);
		}
	}

	void CoreLayer2D_Imp::DrawSpriteAdditionally(Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
		Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
		Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
		Texture2D* texture, AlphaBlend alphaBlend, int32_t priority)
	{
		Sprite sprite;
		std::array<Vector2DF, 4> pos = { upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos };
		std::array<Color, 4> col = { upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol };
		std::array<Vector2DF, 4> uv = { upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV };

		SafeAddRef(texture);

		sprite.pos = pos;
		sprite.col = col;
		sprite.uv = uv;
		sprite.Texture_ = CreateSharedPtrWithReleaseDLL(texture);
		sprite.AlphaBlend_ = alphaBlend;
		sprite.Priority = priority;

		sprites.push_back(sprite);
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

		for (auto& sprite : sprites)
		{
			m_renderer->AddSprite(
				sprite.pos.data(),
				sprite.col.data(),
				sprite.uv.data(),
				sprite.Texture_.get(),
				sprite.AlphaBlend_,
				sprite.Priority);
		}
		sprites.clear();

		DrawObjects(m_renderer);

		if (m_cameras.empty())
		{
			
		}
		else
		{
			m_rendererForCamera->ClearCache();
			for (auto& c : m_cameras)
			{
				c->SetForRenderTarget();
				c->FlushToBuffer(m_renderer);
			}

			for (auto& c : m_cameras)
			{
				c->DrawBuffer(m_rendererForCamera);
			}
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::EndDrawing()
	{
		m_scene->SetRenderTargetForDrawingLayer();

		if (m_cameras.empty())
		{
			m_renderer->SetArea(RectF(0, 0, m_windowSize.X, m_windowSize.Y));
			m_renderer->DrawCache();
			m_renderer->ClearCache();
		}
		else
		{
			
			m_rendererForCamera->SetArea(RectF(0, 0, m_windowSize.X, m_windowSize.Y));
			m_rendererForCamera->DrawCache();
			m_rendererForCamera->ClearCache();
		}

		m_renderer->ClearCache();
		m_rendererForCamera->ClearCache();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::Clear()
	{
		for(auto object:m_objects)
		{
			{
				auto o = CoreObject2DToImp(object);
				o->OnRemoving(m_renderer);
			}
			object->SetLayer(nullptr);
			m_objects.remove(object);
			SafeRelease(object);
		}

		m_objects.clear();

		for (auto object : m_cameras)
		{
			{
				auto o = CoreObject2DToImp(object);
				o->OnRemoving(m_renderer);
			}
			object->SetLayer(nullptr);
			auto camera = (CoreCameraObject2D*)object;
			m_cameras.remove(camera);
			SafeRelease(camera);

		}

		m_cameras.clear();
	}
}