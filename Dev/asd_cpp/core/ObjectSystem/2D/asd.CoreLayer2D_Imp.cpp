
#include "asd.CoreLayer2D_Imp.h"
#include "../../Window/asd.Window_Imp.h"
#include "../../Graphics/asd.Graphics_Imp.h"
#include "../../Graphics/2D/asd.LayerRenderer.h"

#include "../../Graphics/Resource/asd.Font_Imp.h"
#include "../../ObjectSystem/2D/asd.CoreChip2D_Imp.h"
#include "../../Graphics/Resource/asd.Texture2D_Imp.h"
#include "../../Graphics/Resource/asd.Material2D_Imp.h"
#include "../../Graphics/Resource/asd.Shader2D_Imp.h"

#include "asd.CoreTextureObject2D_Imp.h"
#include "asd.CoreTextObject2D_Imp.h"
#include "asd.CoreCameraObject2D_Imp.h"
#include "asd.CoreMapObject2D_Imp.h"
#include "asd.CoreEffectObject2D_Imp.h"
#include "asd.CoreObject2D_Imp.h"
#include "asd.CoreGeometryObject2D_Imp.h"
#include "../../Core/asd.Core.h"
#include "../../ObjectSystem/2D/asd.CoreChip2D_Imp.h"
#include "../../Shape/asd.CoreTriangleShape_Imp.h"
#include "../../Shape/asd.CoreShapeConverter.h"

using namespace std;

namespace asd
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
		WriteLog("Start initializing m_renderer");

		m_renderer = new Renderer2D_Imp(graphics, log);

		WriteLog("End initializing m_renderer");

		WriteLog("Start initializing m_rendererForCamera");

		m_rendererForCamera = new Renderer2D_Imp(graphics, log);

		WriteLog("End initializing m_rendererForCamera");

		WriteLog("Start initializing world");
#if __CULLING_2D__
		world = new culling2d::World(6, culling2d::RectF(-50000, -50000, 100000, 100000));
#endif
		WriteLog("End initializing world");
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreLayer2D_Imp::~CoreLayer2D_Imp()
	{
		ClearAdditionalObjects();

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
					auto chipImp = (CoreChip2D_Imp*)c;

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
		transformedObjects.clear();
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
		case Object2DType::Geometry:
			return (CoreGeometryObject2D_Imp*)obj;
		default:
			break;
		}

		return nullptr;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
#if __CULLING_2D__
	void CoreLayer2D_Imp::AddChipCullingObject(CoreChip2D_Imp *chip, uint32_t firstSortKey)
	{
		auto mapObject = chip->GetMapObject2D();
		auto userData = new Culling2DUserData(mapObject, (CoreChip2D*)chip);

		auto cObj = culling2d::Object::Create(userData, world);
		chip->SetCullingObject(cObj);

		//auto circle = chip->GetBoundingCircle();
		//cObj->SetCircle(circle);

		cObj->SetFirstSortedKey(firstSortKey);
		cObj->SetSecondSortedKey(world->GetNextSecondSortedKey());

		world->IncNextSecondSortedKey();

		AddTransformedObject(cObj);

		chip->SetAlreadyCullingUpdated(true);
		world->AddObject(cObj);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::RemoveChipCullingObject(CoreChip2D_Imp *chip)
	{
		auto cObj = chip->GetCullingObject();

		auto userData = (Culling2DUserData*)(cObj->GetUserData());

		SafeDelete(userData);

		world->RemoveObject(cObj);
	}
#endif

	void CoreLayer2D_Imp::SetSize(Vector2DI size)
	{
		m_windowSize = size;
		m_layerRenderer->SetWindowSize(size);

		{
			asd::Vector2DF lpos[4];
			lpos[0].X = 0;
			lpos[0].Y = 0;
			lpos[1].X = m_windowSize.X;
			lpos[1].Y = 0;
			lpos[2].X = m_windowSize.X;
			lpos[2].Y = m_windowSize.Y;
			lpos[3].X = 0;
			lpos[3].Y = m_windowSize.Y;
			m_layerRenderer->SetLayerPosition(lpos);
		}
	}

	void CoreLayer2D_Imp::PrepareToAddObject(ObjectPtr object)
	{
		auto o = CoreObject2DToImp(object);
		o->OnAdding(m_renderer);
	}

	void CoreLayer2D_Imp::PrepareToRemoveObject(ObjectPtr object)
	{
		auto o = CoreObject2DToImp(object);
		o->OnRemoving(m_renderer);
	}

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
			o->SetIndexInLayer(indexInLayer);
			indexInLayer++;

#if __CULLING_2D__
			if (object->GetObjectType() == Object2DType::Map)
			{
				auto map = (CoreMapObject2D_Imp*)o;
				map->RegisterObjectToCulling();
				map->SetFirstSortedKey(world->GetNextFirstSortedKey());
				world->IncNextFirstSortedKey();
			}
			else if (object->GetObjectType() != Object2DType::Camera)
			{
				auto userData = new Culling2DUserData(object);

				auto cObj = culling2d::Object::Create(userData, world);

				cObj->SetFirstSortedKey(world->GetNextFirstSortedKey());
				world->IncNextFirstSortedKey();

				o->SetCullingObject(cObj);

				//auto circle = o->GetBoundingCircle();
				//cObj->SetCircle(circle);

				AddTransformedObject(cObj);
				o->SetAlreadyCullingUpdated(true);


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
			//o->OnRemoving(m_renderer);

#if __CULLING_2D__
			if (object->GetObjectType() == Object2DType::Map)
			{
				auto mapObj = (CoreMapObject2D_Imp*)o;
				auto &chips = mapObj->GetChips();
				for (auto c : chips)
				{
					auto chipImp = (CoreChip2D_Imp*)c;

					auto cObj = chipImp->GetCullingObject();

					RemoveTransformedObject(cObj);

					auto userData = (Culling2DUserData*)(cObj->GetUserData());

					SafeDelete(userData);

					world->RemoveObject(cObj);
				}
			}
			else if (object->GetObjectType() != Object2DType::Camera)
			{
				auto cObj = o->GetCullingObject();

				RemoveTransformedObject(cObj);

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
	void CoreLayer2D_Imp::BeginUpdating(bool isUpdated)
	{
		for (auto&x : m_objects)
		{
			auto o = CoreObject2DToImp(x);
			o->SetAlreadyCullingUpdated(false);

			if (x->GetObjectType() == Object2DType::Map)
			{
				auto mapObj = (asd::CoreMapObject2D_Imp*)o;
				for (auto chip : mapObj->GetChips())
				{
					chip->SetAlreadyCullingUpdated(false);
				}
				
			}
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
#if __CULLING_2D__
	void CoreLayer2D_Imp::AddTransformedObject(culling2d::Object* object)
	{
		assert(object != nullptr);
		transformedObjects.push_back(object);
	}

	void CoreLayer2D_Imp::RemoveTransformedObject(culling2d::Object* object)
	{
		transformedObjects.erase(std::remove(transformedObjects.begin(), transformedObjects.end(), object), transformedObjects.end());
	}
#endif

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::EndUpdating(bool isUpdated)
	{
		if (isUpdated)
		{
			m_renderer->GetEffectManager()->Update(core->GetDeltaTime() / (1.0f / 60.0f));
			m_renderer->GetEffectManager()->Flip();
		}
		
#if __CULLING_2D__
		//グリッド更新処理
		{
			for (auto x : transformedObjects)
			{
				auto userData = (Culling2DUserData*)(x->GetUserData());

				if (userData->IsObject)
				{
					auto impObj = CoreObject2DToImp(userData->Object);
					impObj->CalculateBoundingCircle();
					auto newCircle = impObj->GetBoundingCircle();
					x->SetCircle(newCircle);
				}
				else
				{
					auto chip = (CoreChip2D_Imp*)userData->Chip;
					auto newCircle = chip->GetBoundingCircle();
					x->SetCircle(newCircle);
				}
			}

			world->Update();

			transformedObjects.clear();
		}
#endif
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::BeginDrawing()
	{

	}
#if __CULLING_2D__
	void CoreLayer2D_Imp::DrawObjectsWithCulling(int32_t group, RectF drawRange)
	{
		std::map<int32_t, CoreObject2D*> renderedObjects;
		
		auto cullingSrc = culling2d::RectF(drawRange.X, drawRange.Y, drawRange.Width, drawRange.Height);
		auto cullingObjects = world->GetCullingObjects(cullingSrc);

		for (auto& culledObj : cullingObjects)
		{
			auto userData = (Culling2DUserData*)(culledObj->GetUserData());

			if (!(userData->Object->GetIsAlive())) continue;
			if ((userData->Object->GetCameraGroup() & group) == 0) continue;

			auto obj_Imp = CoreObject2DToImp(userData->Object);
			renderedObjects[obj_Imp->GetIndexInLayer()] = userData->Object;
		}

		for (auto& o : renderedObjects)
		{
			auto obj_Imp = CoreObject2DToImp(o.second);

			if (o.second->GetObjectType() == Object2DType::Map)
			{
				auto imp = (CoreMapObject2D_Imp*)obj_Imp;
				imp->Culling(cullingObjects);
				imp->Draw(m_renderer);
			}
			else if (o.second->GetObjectType() == Object2DType::Effect)
			{
			}
			else
			{
				o.second->Draw(m_renderer);
			}
		}
	}

#else
	void CoreLayer2D_Imp::DrawObjectsWithoutCulling()
	{
		for (auto& x : m_objects)
		{
			if (x->GetIsAlive())
			{
				x->Draw(m_renderer);
			}
		}
	}
#endif

	void CoreLayer2D_Imp::ClearAdditionalObjects()
	{
		for (auto& s : sprites)
		{
			SafeRelease(s.Texture_);
			SafeRelease(s.Material_);
		}
		sprites.clear();

		texts.clear();
	}

	void CoreLayer2D_Imp::DrawAdditionalObjects(int32_t group)
	{

#if __CULLING_2D__
		//一時的にエフェクトは無条件に描画(本来ここではない)
		for (auto& x : m_objects)
		{
			if (x->GetIsAlive() && 
				(x->GetCameraGroup() & group) != 0 &&
				x->GetObjectType() == Object2DType::Effect)
			{
				x->Draw(m_renderer);
			}
		}
#endif

		for (auto& sprite : sprites)
		{
			if (sprite.Material_ != nullptr)
			{
				m_renderer->AddSpriteWithMaterial(
					sprite.pos.data(),
					sprite.col.data(),
					sprite.uv.data(),
					sprite.Material_,
					sprite.AlphaBlend_,
					sprite.Priority);
			}
			else
			{
				m_renderer->AddSprite(
					sprite.pos.data(),
					sprite.col.data(),
					sprite.uv.data(),
					sprite.Texture_,
					sprite.AlphaBlend_,
					sprite.Priority);
			}

		}

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
				text.Priority_,
				0,
				0);
		}
	}

	void CoreLayer2D_Imp::DrawSpriteAdditionally(
		const Vector2DF& upperLeftPos, const Vector2DF& upperRightPos, const Vector2DF& lowerRightPos, const Vector2DF& lowerLeftPos,
		const Color& upperLeftCol, const Color& upperRightCol, const Color& lowerRightCol, const Color& lowerLeftCol,
		const Vector2DF& upperLeftUV, const Vector2DF& upperRightUV, const Vector2DF& lowerRightUV, const Vector2DF& lowerLeftUV,
		Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		Sprite sprite;
		std::array<Vector2DF, 4> pos = { upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos };
		std::array<Color, 4> col = { upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol };
		std::array<Vector2DF, 4> uv = { upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV };

		SafeAddRef(texture);

		sprite.pos = pos;
		sprite.col = col;
		sprite.uv = uv;
		sprite.Texture_ = texture;
		sprite.Material_ = nullptr;
		sprite.AlphaBlend_ = alphaBlend;
		sprite.Priority = priority;

		sprites.push_back(sprite);
	}

	void CoreLayer2D_Imp::DrawSpriteWithMaterialAdditionally(
		const Vector2DF& upperLeftPos, const Vector2DF& upperRightPos, const Vector2DF& lowerRightPos, const Vector2DF& lowerLeftPos,
		const Color& upperLeftCol, const Color& upperRightCol, const Color& lowerRightCol, const Color& lowerLeftCol,
		const Vector2DF& upperLeftUV, const Vector2DF& upperRightUV, const Vector2DF& lowerRightUV, const Vector2DF& lowerLeftUV,
		Material2D* material, AlphaBlendMode alphaBlend, int32_t priority)
	{
		Sprite sprite;
		std::array<Vector2DF, 4> pos = { upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos };
		std::array<Color, 4> col = { upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol };
		std::array<Vector2DF, 4> uv = { upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV };

		SafeAddRef(material);

		sprite.pos = pos;
		sprite.col = col;
		sprite.uv = uv;
		sprite.Texture_ = nullptr;
		sprite.Material_ = material;
		sprite.AlphaBlend_ = alphaBlend;
		sprite.Priority = priority;

		sprites.push_back(sprite);
	}

	void CoreLayer2D_Imp::DrawTextAdditionally(Vector2DF pos, Color color, Font* font, const achar* text, WritingDirection writingDirection, AlphaBlendMode alphaBlend, int32_t priority)
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

		if (m_cameras.empty())
		{

#if __CULLING_2D__
			auto range = RectF(0, 0, m_windowSize.X, m_windowSize.Y);
			DrawObjectsWithCulling(INT_MAX,range);
#else
			DrawObjectsWithoutCulling();
#endif

			DrawAdditionalObjects(INT_MAX);
			ClearAdditionalObjects();
		}
		else
		{
#if __CULLING_2D__
			for (auto& c : m_cameras)
			{
				if (!c->GetIsDrawn())
					continue;
					
				//レンダラのキャッシュをリセット
				m_renderer->ClearCache();

				//通常のオブジェクト摘み取りと描画
				auto src = c->GetSrc();
				DrawObjectsWithCulling(c->GetCameraGroup(), asd::RectF(src.X, src.Y, src.Width, src.Height));

				//追加オブジェクト描画
				DrawAdditionalObjects(c->GetCameraGroup());

				//カメラバッファに内容をセット
				c->SetForRenderTarget();
				c->FlushToBuffer(m_renderer);

				if (!c->GetIsOffscreenMode())
				{
					c->DrawBuffer(m_rendererForCamera);
				}
			}

			ClearAdditionalObjects();
#else

			DrawObjectsWithoutCulling();

			DrawAdditionalObjects();
			ClearAdditionalObjects();

			for (auto& c : m_cameras)
			{
				c->SetForRenderTarget();
				c->FlushToBuffer(m_renderer);

				c->DrawBuffer(m_rendererForCamera);
			}

#endif
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
			m_renderer->IsDistortionEnabled = isDistortionEnabled;
			m_renderer->DrawCache();
			m_renderer->ClearCache();
		}
		else
		{

			m_rendererForCamera->SetArea(RectF(0, 0, m_windowSize.X, m_windowSize.Y), 0.0f);
			m_rendererForCamera->IsDistortionEnabled = isDistortionEnabled;
			m_rendererForCamera->DrawCache();
			m_rendererForCamera->ClearCache();
		}

		m_renderer->ClearCache();
		m_rendererForCamera->ClearCache();

	}

	bool CoreLayer2D_Imp::GetIsDistortionEnabled() const
	{
		return isDistortionEnabled;
	}

	void CoreLayer2D_Imp::SetIsDistortionEnabled(bool value)
	{
		isDistortionEnabled = value;
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
						auto chipImp = (CoreChip2D_Imp*)c;

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

#if __CULLING_2D__
		transformedObjects.clear();
		world->ResetNextFirstSortedKey();
		world->ResetNextSecondSortedKey();
#endif

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

	void CoreLayer2D_Imp::DrawRectangleAdditionally(const RectF& drawingArea, const Color& color, const RectF& uv, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		Sprite sprite;

		std::array<Color, 4> col = { color, color, color, color };

		SafeAddRef(texture);

		sprite.pos = drawingArea.GetVertexes();
		sprite.col = col;
		sprite.uv = uv.GetVertexes();
		sprite.Texture_ = texture;
		sprite.Material_ = nullptr;
		sprite.AlphaBlend_ = alphaBlend;
		sprite.Priority = priority;

		sprites.push_back(sprite);
	}

	void CoreLayer2D_Imp::DrawRotatedRectangleAdditionally(RectF drawingArea, Color color, Vector2DF rotationCenter, float angle, RectF uv, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		Sprite sprite;

		std::array<Color, 4> col = { color, color, color, color };

		SafeAddRef(texture);

		auto vertexes = drawingArea.GetVertexes();

		auto globalCenter = vertexes[0] + rotationCenter;

		for (auto& vert : vertexes)
		{
			vert -= globalCenter;
			auto deg = vert.GetDegree();
			deg += angle;
			vert.SetDegree(deg);
			vert += globalCenter;
		}


		sprite.pos = vertexes;
		sprite.col = col;
		sprite.uv = uv.GetVertexes();
		sprite.Texture_ = texture;
		sprite.Material_ = nullptr;
		sprite.AlphaBlend_ = alphaBlend;
		sprite.Priority = priority;

		sprites.push_back(sprite);
	}

	void CoreLayer2D_Imp::DrawTriangleAdditionally(Vector2DF position1, Vector2DF position2, Vector2DF position3, Color color, Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		Sprite sprite;

		std::array<Vector2DF, 4> vertexes = { position1, position2, position3, position3 };
		std::array<Vector2DF, 4> uvs = { uv1, uv2, uv3, uv3 };

		std::array<Color, 4> col = { color, color, color, color };

		SafeAddRef(texture);

		sprite.pos = vertexes;
		sprite.col = col;
		sprite.uv = uvs;
		sprite.Texture_ = texture;
		sprite.Material_ = nullptr;
		sprite.AlphaBlend_ = alphaBlend;
		sprite.Priority = priority;

		sprites.push_back(sprite);
	}

	void CoreLayer2D_Imp::DrawCircleAdditionally(asd::Vector2DF center, float outerDiameter, float innerDiameter, Color color, int numberOfCorners, float angle, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (numberOfCorners < 3) return;

		const float radInc = 360.0 / numberOfCorners;

		const float outerRadius = outerDiameter / 2;
		const float innerRadius = innerDiameter / 2;

		float currentPosDeg = angle - 90;
		float currentUVDeg = -90;

		Vector2DF baseVector(0, -1);
		baseVector.SetDegree(currentPosDeg);

		Vector2DF uvCenter = { 0.5, 0.5 };

		Vector2DF uvVector = { 0, -0.5 };

		float ratio = innerDiameter / outerDiameter;

		for (int i = 0; i < numberOfCorners; ++i)
		{
			Vector2DF currentPosVector = baseVector;
			currentPosVector.SetDegree(currentPosDeg);

			Vector2DF nextPosVector = currentPosVector;
			nextPosVector.SetDegree(currentPosDeg + radInc);

			Vector2DF currentUVVector = uvVector;
			currentUVVector.SetDegree(currentUVDeg);

			Vector2DF nextUVVector = currentUVVector;
			nextUVVector.SetDegree(currentUVDeg + radInc);


			std::array<Vector2DF, 4> vertexes = { center + currentPosVector*outerRadius, center + nextPosVector*outerRadius, center + nextPosVector*innerRadius, center + currentPosVector*innerRadius };
			std::array<Vector2DF, 4> uvs = { uvCenter + currentUVVector, uvCenter + nextUVVector, uvCenter + nextUVVector*ratio, uvCenter + currentUVVector*ratio };
			std::array<Color, 4> colors = { color, color, color, color };

			SafeAddRef(texture);

			{
				Sprite sprite;

				sprite.pos = vertexes;
				sprite.col = colors;
				sprite.uv = uvs;
				sprite.Texture_ = texture;
				sprite.Material_ = nullptr;
				sprite.AlphaBlend_ = alphaBlend;
				sprite.Priority = priority;

				sprites.push_back(sprite);
			}

			currentPosDeg += radInc;
			currentUVDeg += radInc;
		}
	}

	void CoreLayer2D_Imp::DrawArcAdditionally(asd::Vector2DF center, float outerDiameter, float innerDiameter, Color color, int numberOfCorners, int startingCorner, int endingCorner, float angle, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (numberOfCorners < 3) return;

		const float radInc = 360.0 / numberOfCorners;

		const float outerRadius = outerDiameter / 2;
		const float innerRadius = innerDiameter / 2;

		float currentPosDeg = angle - 90 + startingCorner*radInc;
		float currentUVDeg = -90 + startingCorner*radInc;

		Vector2DF baseVector(0, -1);
		baseVector.SetDegree(currentPosDeg);

		Vector2DF uvCenter = { 0.5, 0.5 };

		Vector2DF uvVector = { 0, -0.5 };

		float ratio = innerDiameter / outerDiameter;

		int endcorner = endingCorner;
		while (endcorner < startingCorner) endcorner += numberOfCorners;

		for (int i = 0; i < endcorner - startingCorner; ++i)
		{
			Vector2DF currentPosVector = baseVector;
			currentPosVector.SetDegree(currentPosDeg);

			Vector2DF nextPosVector = currentPosVector;
			nextPosVector.SetDegree(currentPosDeg + radInc);

			Vector2DF currentUVVector = uvVector;
			currentUVVector.SetDegree(currentUVDeg);

			Vector2DF nextUVVector = currentUVVector;
			nextUVVector.SetDegree(currentUVDeg + radInc);


			std::array<Vector2DF, 4> vertexes = { center + currentPosVector*outerRadius, center + nextPosVector*outerRadius, center + nextPosVector*innerRadius, center + currentPosVector*innerRadius };
			std::array<Vector2DF, 4> uvs = { uvCenter + currentUVVector, uvCenter + nextUVVector, uvCenter + nextUVVector*ratio, uvCenter + currentUVVector*ratio };
			std::array<Color, 4> colors = { color, color, color, color };

			SafeAddRef(texture);

			{
				Sprite sprite;

				sprite.pos = vertexes;
				sprite.col = colors;
				sprite.uv = uvs;
				sprite.Texture_ = texture;
				sprite.Material_ = nullptr;
				sprite.AlphaBlend_ = alphaBlend;
				sprite.Priority = priority;

				sprites.push_back(sprite);
			}

			currentPosDeg += radInc;
			currentUVDeg += radInc;
		}
	}

	void CoreLayer2D_Imp::DrawLineAdditionally(Vector2DF point1, Vector2DF point2, float thickness, Color color, AlphaBlendMode alphaBlend, int32_t priority)
	{
		Vector2DF vector = point2 - point1;

		auto binorm = vector;
		{
			auto deg = binorm.GetDegree();
			deg += 90;
			binorm.SetDegree(deg);
			binorm.Normalize();
		}

		auto halfThickness = thickness / 2;
		
		std::array<Vector2DF, 4> pos = { point1 + binorm*halfThickness, point2 + binorm*halfThickness, point2 - binorm*halfThickness, point1 - binorm*halfThickness };
		std::array<Color, 4> col = { color, color, color, color };
		std::array<Vector2DF, 4> uv = { Vector2DF(0, 0), Vector2DF(0, 0), Vector2DF(0, 0), Vector2DF(0, 0) };

		Sprite sprite;

		sprite.pos = pos;
		sprite.col = col;
		sprite.uv = uv;
		sprite.Texture_ = nullptr;
		sprite.AlphaBlend_ = alphaBlend;
		sprite.Priority = priority;

		sprites.push_back(sprite);
	}

	void CoreLayer2D_Imp::DrawShapeAdditionally(CoreShape* shape, Color color, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		auto shape_Imp = CoreShape2DToImp(shape);
		for (auto triangle : shape_Imp->GetDividedTriangles())
		{
			std::array<Vector2DF, 4> pos;
			std::array<Vector2DF, 4> uvs;

			for (int i = 0; i < 3; ++i)
			{
				pos[i] = triangle->GetPointByIndex(i);
				uvs[i] = triangle->GetUVByIndex(i);
			}
			pos[3] = pos[2];
			uvs[3] = uvs[2];

			std::array<Color,4> col;
			col[0] = color;
			col[1] = color;
			col[2] = color;
			col[3] = color;

			Sprite sprite;

			sprite.pos = pos;
			sprite.col = col;
			sprite.uv = uvs;
			if (shape->GetShapeType() == ShapeType::LineShape)
			{
				sprite.Texture_ = nullptr;
			}
			else
			{
				sprite.Texture_ =  texture;
				SafeAddRef(texture);
			}

			sprite.Material_ = nullptr;
			
			sprite.AlphaBlend_ = alphaBlend;
			sprite.Priority = priority;

			sprites.push_back(sprite);
		}
	}
}