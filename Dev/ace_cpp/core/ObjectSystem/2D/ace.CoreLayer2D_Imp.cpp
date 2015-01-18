
#include "ace.CoreLayer2D_Imp.h"
#include "../../Window/ace.Window_Imp.h"
#include "../../Graphics/ace.Graphics_Imp.h"
#include "../../Graphics/2D/ace.LayerRenderer.h"

#include "../../Graphics/Resource/ace.Font_Imp.h"
#include "../../Graphics/Resource/ace.Chip2D_Imp.h"
#include "../../Graphics/Resource/ace.Texture2D_Imp.h"

#include "ace.CoreTextureObject2D_Imp.h"
#include "ace.CoreTextObject2D_Imp.h"
#include "ace.CoreCameraObject2D_Imp.h"
#include "ace.CoreMapObject2D_Imp.h"
#include "ace.CoreEffectObject2D_Imp.h"
#include "ace.CoreObject2D_Imp.h"
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

#if __CULLING_2D__
		world = new culling2d::World(6, culling2d::RectF(0, 0, 100000, 100000));
#endif
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
#if __CULLING_2D__
			if (object->GetObjectType() == Object2DType::Map)
			{
				auto mapObj = (CoreMapObject2D_Imp*)CoreObject2DToImp(object);
				auto &chips = mapObj->GetChips();
				for (auto c : chips)
				{
					auto chipImp = (Chip2D_Imp*)c;

					auto cObj = chipImp->GetCullingObject();

					auto userData = (Culling2DUserData*)(cObj->GetUserData());

					SafeDelete(userData);

					world->RemoveObject(cObj);
				}
			}
			else if (object->GetObjectType() != Object2DType::Camera)
			{
				auto o = CoreObject2DToImp(object);
				auto cObj = o->GetCullingObject();
				auto userData = (Culling2DUserData*)(cObj->GetUserData());

				SafeDelete(userData);

				world->RemoveObject(cObj);
			}
#endif
			
			object->SetLayer(nullptr);
			SafeRelease(object);
		}

		for (auto& camera : m_cameras)
		{
			camera->SetLayer(nullptr);
			SafeRelease(camera);
		}
#if __CULLING_2D__
		culling2d::SafeDelete(world);
#endif
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

#if __CULLING_2D__
			if (object->GetObjectType() == Object2DType::Map)
			{
				auto map = (CoreMapObject2D_Imp*)o;
				map->RegisterObjectToCulling();
			}
			else
			{
				auto userData = new Culling2DUserData(object);

				o->CalculateBoundingCircle();

				auto cObj = new culling2d::Object(o->GetBoundingCircle() , userData, world);

				o->SetCullingObject(cObj);

				world->AddObject(cObj);
			}
#endif
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

#if __CULLING_2D__
			if (object->GetObjectType() == Object2DType::Map)
			{
				auto mapObj = (CoreMapObject2D_Imp*)o;
				auto &chips = mapObj->GetChips();
				for (auto c : chips)
				{
					auto chipImp = (Chip2D_Imp*)c;

					auto cObj = chipImp->GetCullingObject();

					auto userData = (Culling2DUserData*)(cObj->GetUserData());

					SafeDelete(userData);

					world->RemoveObject(cObj);
				}
			}
			else
			{
				auto cObj = o->GetCullingObject();
				auto userData = (Culling2DUserData*)(cObj->GetUserData());

				SafeDelete(userData);

				world->RemoveObject(cObj);
			}
#endif
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
		for (auto&x : m_objects)
		{
			auto o = CoreObject2DToImp(x);
			o->SetAlreadyCullingUpdated(x->GetObjectType() == Object2DType::Map);
		}

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
#if __CULLING_2D__
	culling2d::World *CoreLayer2D_Imp::GetCullingWorld() const
	{
		return world;
	}
#endif

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::EndUpdating()
	{
		m_renderer->GetEffectManager()->Update(core->GetDeltaTime() / (1.0f / 60.0f));
		m_renderer->GetEffectManager()->Flip();

#if __CULLING_2D__
		//グリッド更新処理
		{
			for (auto& x : TransformedObjects)
			{

				auto userData = (Culling2DUserData*)(x->GetUserData());

				if (userData->IsObject)
				{
					auto impObj = CoreObject2DToImp(userData->Object);
					impObj->CalculateBoundingCircle();
					auto newCircle = impObj->GetBoundingCircle();
					x->SetCircle(newCircle);
					world->NotifyMoved(x);
					impObj->SetAlreadyCullingUpdated(true);
				}
				else
				{
					auto impObj = CoreObject2DToImp(userData->Object);
					auto chip = (Chip2D_Imp*)userData->Chip;
					auto newCircle = chip->GetBoundingCircle();
					x->SetCircle(newCircle);
					world->NotifyMoved(x);
					chip->SetAlreadyCullingUpdated(true);
				}
			}

			world->Update();

			TransformedObjects.clear();
		}
#endif
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
#if __CULLING_2D__

		std::vector<culling2d::Object*> allCulledObjects;

		if (m_cameras.size() > 0)
		{
			for (auto& camera : m_cameras)
			{
				auto src = camera->GetSrc();
				auto cullingSrc = culling2d::RectF(src.X, src.Y, src.Width, src.Height);
				auto cullingObjects = world->GetCullingObjects(cullingSrc);

				for (auto& cullingObject : cullingObjects)
				{
					allCulledObjects.push_back(cullingObject);
				}
			}
		}
		else
		{
			auto src = RectF(0, 0, m_windowSize.X, m_windowSize.Y);
			auto cullingSrc = culling2d::RectF(src.X, src.Y, src.Width, src.Height);
			auto cullingObjects = world->GetCullingObjects(cullingSrc);

			for (auto& cullingObject : cullingObjects)
			{
				allCulledObjects.push_back(cullingObject);
			}
		}

		for (auto& culledObj : allCulledObjects)
		{
			auto userData = (Culling2DUserData*)(culledObj->GetUserData());

			if (userData->IsObject&&userData->Object->GetIsAlive())
			{
				auto obj = userData->Object;
				obj->Draw(renderer);
			}
			else if(userData->Object->GetIsAlive())
			{
				auto mapObj = (CoreMapObject2D_Imp*)CoreObject2DToImp(userData->Object);
				auto chip = userData->Chip;

				mapObj->DrawChip(renderer, chip);
			}

		}

		//一時的にエフェクトは無条件に描画
		for (auto& x : m_objects)
		{
			if (x->GetIsAlive() && x->GetObjectType() == Object2DType::Effect)
			{
				x->Draw(renderer);
			}
		}

#else
		for (auto& x : m_objects)
		{
			if (x->GetIsAlive())
			{
				x->Draw(renderer);
			}
		}
#endif
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

	void CoreLayer2D_Imp::DrawTextAdditionally(Vector2DF pos, Color color, Font* font, const achar* text, WritingDirection writingDirection, AlphaBlend alphaBlend, int32_t priority)
	{
		SafeAddRef(font);

		Text text_;
		text_.Position_ = pos;
		text_.Color_ = color;
		text_.Font_ = CreateSharedPtrWithReleaseDLL(font);
		text_.Text_ = text;
		text_.WritingDirection_ = writingDirection;
		text_.AlphaBlend_ = alphaBlend;
		text_.Priority_ = priority;

		texts.push_back(text_);
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

		for (auto& text : texts)
		{
			Matrix33 matP;
			Matrix33 mat;
			mat.SetTranslation(text.Position_.X, text.Position_.Y);

			m_renderer->AddText(
				matP,
				mat,
				Vector2DF(),
				false,
				false,
				text.Color_,
				text.Font_.get(),
				text.Text_.c_str(),
				text.WritingDirection_,
				text.AlphaBlend_,
				text.Priority_);
		}
		texts.clear();

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
			m_renderer->SetArea(RectF(0, 0, m_windowSize.X, m_windowSize.Y), 0.0f);
			m_renderer->DrawCache();
			m_renderer->ClearCache();
		}
		else
		{
			
			m_rendererForCamera->SetArea(RectF(0, 0, m_windowSize.X, m_windowSize.Y), 0.0f);
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

#if __CULLING_2D__
				if (object->GetObjectType() == Object2DType::Map)
				{
					auto mapObj = (CoreMapObject2D_Imp*)o;
					auto &chips = mapObj->GetChips();
					for (auto c : chips)
					{
						auto chipImp = (Chip2D_Imp*)c;

						auto cObj = chipImp->GetCullingObject();

						auto userData = (Culling2DUserData*)(cObj->GetUserData());

						SafeDelete(userData);

						world->RemoveObject(cObj);
					}
				}
				else if (object->GetObjectType() != Object2DType::Camera)
				{
					auto cObj = o->GetCullingObject();
					auto userData = (Culling2DUserData*)(cObj->GetUserData());

					SafeDelete(userData);

					world->RemoveObject(cObj);
				}
#endif
			}
			object->SetLayer(nullptr);
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
			SafeRelease(camera);

		}

		m_cameras.clear();
	}
}