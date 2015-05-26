#include "ace.Chip2D.h"
#include "../../ace.CoreToEngine.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	Chip2D::Chip2D()
		:m_texture(nullptr)
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateChip2D());
	}

	void Chip2D::SetTexture(std::shared_ptr<Texture2D> texture)
	{
		m_texture = texture;
		m_coreObject->SetTexture(m_texture.get());
	}

	std::shared_ptr<CoreChip2D> Chip2D::GetCoreObject() const
	{
		return m_coreObject;
	}

	std::shared_ptr<Texture2D> Chip2D::GetTexture()
	{
		return m_texture;
	}

	RectF Chip2D::GetSrc() const
	{
		return m_coreObject->GetSrc();
	}

	void Chip2D::SetSrc(RectF src)
	{
		m_coreObject->SetSrc(src);
	}

	Vector2DF Chip2D::GetPosition() const
	{
		return m_coreObject->GetPosition();
	}

	void Chip2D::SetPosition(Vector2DF position)
	{
		m_coreObject->SetPosition(position);
	}

	float Chip2D::GetAngle() const
	{
		return m_coreObject->GetAngle();
	}

	void Chip2D::SetAngle(float angle)
	{
		m_coreObject->SetAngle(angle);
	}

	Vector2DF Chip2D::GetScale() const
	{
		return m_coreObject->GetScale();
	}

	void Chip2D::SetScale(Vector2DF scale)
	{
		m_coreObject->SetScale(scale);
	}

	Vector2DF Chip2D::GetCenterPosition() const
	{
		return m_coreObject->GetCenterPosition();
	}

	void Chip2D::SetCenterPosition(Vector2DF position)
	{
		m_coreObject->SetCenterPosition(position);
	}

	Color Chip2D::GetColor() const
	{
		return m_coreObject->GetColor();
	}

	void Chip2D::SetColor(Color color)
	{
		m_coreObject->SetColor(color);
	}

	bool Chip2D::GetTurnLR() const
	{
		return m_coreObject->GetTurnLR();
	}

	void Chip2D::SetTurnLR(bool turnLR)
	{
		m_coreObject->SetTurnLR(turnLR);
	}

	bool Chip2D::GetTurnUL() const
	{
		return m_coreObject->GetTurnUL();
	}

	void Chip2D::SetTurnUL(bool turnUL)
	{
		m_coreObject->SetTurnUL(turnUL);
	}

	int Chip2D::GetDrawingPriority() const
	{
		return m_coreObject->GetDrawingPriority();
	}

	void Chip2D::SetDrawingPriority(int priority)
	{
		m_coreObject->SetDrawingPriority(priority);
	}

	AlphaBlendMode Chip2D::GetAlphaBlendMode() const
	{
		return m_coreObject->GetAlphaBlendMode();
	}

	void Chip2D::SetAlphaBlendMode(AlphaBlendMode alphaBlend)
	{
		m_coreObject->SetAlphaBlendMode(alphaBlend);
	}

	void Chip2D::SetTextureFilterType(TextureFilterType textureFilterType)
	{
		m_coreObject->SetTextureFilterType(textureFilterType);
	}

	TextureFilterType Chip2D::GetTextureFilterType() const
	{
		return m_coreObject->GetTextureFilterType();
	}
}