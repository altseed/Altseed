
#include "asd.CoreObject2D_Imp.h"
#include "../../Graphics/asd.Graphics_Imp.h"
#include "asd.CoreTextureObject2D_Imp.h"
#include "asd.CoreCameraObject2D_Imp.h"
#include "asd.CoreTextObject2D_Imp.h"
#include "asd.CoreMapObject2D_Imp.h"
#include "asd.CoreEffectObject2D_Imp.h"
#include "asd.CoreGeometryObject2D_Imp.h"
#include <list>

namespace asd
{

	CoreObject2D_Imp::CoreObject2D_Imp(Graphics_Imp* graphics)
		: m_graphics(graphics)
		, m_objectInfo(ObjectInfo2D())
		, m_transform(TransformInfo2D())
		, cullingObject(nullptr)
		, m_boundingCircle(culling2d::Circle())
	{
		SafeAddRef(m_graphics);
		alreadyCullingUpdated = false;
	}

	CoreObject2D_Imp::~CoreObject2D_Imp()
	{
		SafeRelease(m_graphics);
	}

	void CoreObject2D_Imp::SetCullingUpdate(CoreObject2D_Imp* obj)
	{
#if __CULLING_2D__

		{	// 破棄された子を削除する処理のブロック
			std::list<CoreObject2D*> beRemoved = std::list<CoreObject2D*>();

			for (auto& child : obj->children)
			{
				if (!child->GetIsAlive())
				{
					beRemoved.push_back(child);
				}
			}

			for (auto& rm : beRemoved)
			{
				obj->children.erase(rm);
			}
		}

		if (!obj->GetAlreadyCullingUpdated())
		{
			if (obj->GetLayer() != nullptr&&obj->GetIsAlive())
			{
				auto layerImp = (CoreLayer2D_Imp*)obj->GetLayer();
				layerImp->AddTransformedObject(obj->GetCullingObject());
			}
			obj->SetAlreadyCullingUpdated(true);
		}

		for (auto& cld : obj->children)
		{
			auto cld_Imp = CoreObject2DToImp(cld);
			SetCullingUpdate(cld_Imp);
		}
#endif
	}

	culling2d::Circle& CoreObject2D_Imp::GetBoundingCircle()
	{
		return m_boundingCircle;
	}

	CoreObject2D_Imp* CoreObject2D_Imp::CoreObject2DToImp(CoreObject2D* obj)
	{
		if (obj == nullptr) return nullptr;

		switch (obj->GetObjectType())
		{
		case Object2DType::Texture:
			return (CoreTextureObject2D_Imp*)obj;
		case Object2DType::Text:
			return (CoreTextObject2D_Imp*)obj;
		case Object2DType::Camera:
			return (CoreCameraObject2D_Imp*)obj;
		case Object2DType::Map:
			return (CoreMapObject2D_Imp*)obj;
		case Object2DType::Effect:
			return (CoreEffectObject2D_Imp*)obj;
		case Object2DType::Geometry:
			return (CoreGeometryObject2D_Imp*)obj;
		default:
			break;
		}

		return nullptr;
	}


	Vector2DF CoreObject2D_Imp::GetAbsolutePosition()
	{
		auto vec3 = Vector3DF(0, 0, 1);
		auto abs = GetAbsoluteMatrixToTransform();
		auto result = abs * vec3;
		return Vector2DF(result.X, result.Y);
	}

	Matrix33 CoreObject2D_Imp::GetAbsoluteMatrixToTranslate()
	{
		if (m_parentInfo != nullptr)
		{
			return m_parentInfo->GetInheritedMatrixToTranslate() * GetMatrixToTranslate();
		}
		
		return m_transform.GetMatrixToTranslate();
	}

	Matrix33 CoreObject2D_Imp::GetAbsoluteMatrixToTransform()
	{
		if (m_parentInfo != nullptr)
		{
			return m_parentInfo->GetInheritedMatrixToTransform() * GetMatrixToTransform();
		}

		return m_transform.GetMatrixToTransform();
	}

	bool CoreObject2D_Imp::GetAbsoluteBeingDrawn() const
	{
		if (m_parentInfo != nullptr)
		{
			return m_objectInfo.GetIsDrawn() && m_parentInfo->GetInheritedBeingDrawn();
		}
		return m_objectInfo.GetIsDrawn();
	}
}