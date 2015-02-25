
#include "ace.Object3D.h"
#include "ace.Layer3D.h"

namespace ace
{
	void Object3D::Start()
	{
		OnStart();
	}

	void Object3D::Update()
	{
		if (!m_isUpdated)
		{
			return;
		}

		OnUpdate();

		OnUpdateInternal();

		/*
		for (auto& x : m_components)
		{
			x.second->Update();
		}
		*/
	}

	void Object3D::CallDestroy()
	{
		OnDispose();
	}

	void Object3D::SetLayer(Layer3D* layer)
	{
		m_owner = layer;
	}

	CoreObject3D* Object3D::GetCoreObject() const
	{
		return m_commonObject;
	}

	void Object3D::OnVanish()
	{
	}

	void Object3D::OnDispose()
	{
	}

	Object3D::Object3D()
		: m_owner(nullptr)
		, m_commonObject(nullptr)
		, m_isUpdated(true)
		, m_isDrawn(true)
		, m_isAlive(true)
	{

	}

	Object3D::~Object3D()
	{

	}

	Layer3D* Object3D::GetLayer() const
	{
		return m_owner;
	}


	bool Object3D::GetIsUpdated() const
	{
		return m_isUpdated;
	}

	void Object3D::SetIsUpdated(bool value)
	{
		m_isUpdated = value;
	}

	bool Object3D::GetIsDrawn() const
	{
		return m_isDrawn;
	}

	void Object3D::SetIsDrawn(bool value)
	{
		m_isDrawn = value;
		GetCoreObject()->SetIsDrawn(value);
	}

	bool Object3D::GetIsAlive() const
	{
		return m_isAlive;
	}

	void Object3D::Vanish()
	{
		m_isAlive = false;
		OnVanish();
	}

	Vector3DF Object3D::GetPosition() const
	{
		return m_commonObject->GetPosition();
	}

	void Object3D::SetPosition(Vector3DF position)
	{
		m_commonObject->SetPosition(position);
	}

	Vector3DF Object3D::GetRotation() const
	{
		return m_commonObject->GetRotation();
	}

	void Object3D::SetRotation(Vector3DF angle)
	{
		m_commonObject->SetRotation(angle);
	}

	Vector3DF Object3D::GetScale() const
	{
		return m_commonObject->GetScale();
	}

	void Object3D::SetScale(Vector3DF scale)
	{
		m_commonObject->SetScale(scale);
	}

	void Object3D::DrawSpriteAdditionally(Vector3DF upperLeftPos, Vector3DF upperRightPos, Vector3DF lowerRightPos, Vector3DF lowerLeftPos,
		Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
		Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
		std::shared_ptr<Texture2D>  texture, AlphaBlend alphaBlend, bool depthWrite, bool depthTest)
	{
		auto layer = GetLayer();
		layer->DrawSpriteAdditionally(
			upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos,
			upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol,
			upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV,
			texture, alphaBlend, depthWrite, depthTest);
	}
}