//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../Graphics/asd.Graphics_Imp.h"
#include "../../ObjectSystem/2D/asd.CoreMapObject2D_Imp.h"
#include "asd.CoreChip2D_Imp.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreChip2D_Imp::CoreChip2D_Imp(Graphics* graphics)
		: m_graphics(graphics)
		, m_src(RectF(0,0,1,1))
		, m_color(Color())
		, m_alphablend(AlphaBlendMode::Blend)
		, m_texture(nullptr)
		, m_turnLR(false)
		, m_turnUL(false)
		, mapObject2D(nullptr)
		, m_drawingPriority(0)
		, m_transformInfo(TransformInfo2D())
		, m_textureFilterType(TextureFilterType::Nearest)
		, m_centerPosition(Vector2DF())
#if __CULLING_2D__
		, cullingObject(nullptr)
		, alreadyCullingUpdated(false)
#endif
	{

	}

	TransformInfo2D CoreChip2D_Imp::GetTransformInfo2D() const
	{
		return m_transformInfo;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreMapObject2D_Imp* CoreChip2D_Imp::GetMapObject2D() const
	{
		return mapObject2D;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreChip2D_Imp::SetMapObject2D(CoreMapObject2D_Imp* mapObject)
	{
		mapObject2D = mapObject;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreChip2D_Imp::~CoreChip2D_Imp()
	{
		SafeRelease(m_texture);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
#if __CULLING_2D__
	bool CoreChip2D_Imp::GetAlreadyCullingUpdated() const
	{
		return alreadyCullingUpdated;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreChip2D_Imp::SetCullingObject(culling2d::Object *cullingObj)
	{
		cullingObject = cullingObj;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	culling2d::Object* CoreChip2D_Imp::GetCullingObject() const
	{
		return cullingObject;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreChip2D_Imp::SetAlreadyCullingUpdated(bool cullingUpdated)
	{
		alreadyCullingUpdated = cullingUpdated;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	culling2d::Circle CoreChip2D_Imp::GetBoundingCircle()
	{
		return mapObject2D->GetChipBoundingCircle(this);
	}
#endif

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D* CoreChip2D_Imp::GetTexture_() const
	{
		return m_texture;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreChip2D_Imp::SetTexture(Texture2D* texture)
	{
		SafeSubstitute(m_texture, texture);
		if (texture != nullptr)
		{
			SetSrc(RectF(0, 0, texture->GetSize().X, texture->GetSize().Y));
		}
		else
		{
			m_src = RectF(0, 0, 1, 1);
		}
	}

#if !SWIG
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreChip2D_Imp::SetTexture(std::shared_ptr<Texture2D> texture)
	{
		SetTexture(texture.get());
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	std::shared_ptr<Texture2D> CoreChip2D_Imp::GetTexture()
	{
		auto v = GetTexture_();
		SafeAddRef(v);
		return CreateSharedPtrWithReleaseDLL(v);
	}

#endif

	Vector2DF CoreChip2D_Imp::GetPosition() const
	{
		return m_transformInfo.GetPosition();
	}

	void CoreChip2D_Imp::SetPosition(Vector2DF position)
	{
		m_transformInfo.SetPosition(position);
		SetCullingUpdate();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RectF CoreChip2D_Imp::GetSrc() const
	{
		return m_src;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreChip2D_Imp::SetSrc(RectF src)
	{
		m_src = src;
	}

	void CoreChip2D_Imp::SetCullingUpdate()
	{
#if __CULLING_2D__

		if (!alreadyCullingUpdated&&mapObject2D != nullptr&&mapObject2D->GetLayer() != nullptr)
		{
			auto layerImp = (CoreLayer2D_Imp*)mapObject2D->GetLayer();
			layerImp->AddTransformedObject(cullingObject);
			alreadyCullingUpdated = true;
		}
#endif
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Color CoreChip2D_Imp::GetColor() const
	{
		return m_color;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreChip2D_Imp::SetColor(Color color)
	{
		m_color = color;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool CoreChip2D_Imp::GetTurnLR() const
	{
		return m_turnLR;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreChip2D_Imp::SetTurnLR(bool turnLR)
	{
		m_turnLR = turnLR;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool CoreChip2D_Imp::GetTurnUL() const
	{
		return m_turnUL;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreChip2D_Imp::SetTurnUL(bool turnUL)
	{
		m_turnUL = turnUL;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	AlphaBlendMode CoreChip2D_Imp::GetAlphaBlendMode() const
	{
		return m_alphablend;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreChip2D_Imp::SetAlphaBlendMode(AlphaBlendMode alphaBlend)
	{
		m_alphablend = alphaBlend;
	}

	float CoreChip2D_Imp::GetAngle() const
	{
		return m_transformInfo.GetAngle();
	}

	void CoreChip2D_Imp::SetAngle(float angle)
	{
		m_transformInfo.SetAngle(angle);
		SetCullingUpdate();
	}

	Vector2DF CoreChip2D_Imp::GetScale() const
	{
		return m_transformInfo.GetScale();
	}

	void CoreChip2D_Imp::SetScale(Vector2DF scale)
	{
		m_transformInfo.SetScale(scale);
		SetCullingUpdate();
	}

	Vector2DF CoreChip2D_Imp::GetCenterPosition() const
	{
		return m_centerPosition;
	}

	void CoreChip2D_Imp::SetCenterPosition(Vector2DF position)
	{
		m_centerPosition = position;
		SetCullingUpdate();
	}

	int CoreChip2D_Imp::GetDrawingPriority() const
	{
		return m_drawingPriority;
	}

	void CoreChip2D_Imp::SetDrawingPriority(int priority)
	{
		m_drawingPriority = priority;
	}

	void CoreChip2D_Imp::SetTextureFilterType(TextureFilterType textureFilterType)
	{
		m_textureFilterType = textureFilterType;
	}

	TextureFilterType CoreChip2D_Imp::GetTextureFilterType() const
	{
		return m_textureFilterType;
	}
}