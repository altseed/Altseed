#pragma once

#include "../../ace.CoreToEngine.h"

namespace ace
{
	class MapObject2D;

	class Chip2D
	{
		friend class MapObject2D;
	private:
		std::shared_ptr<Texture2D> m_texture;
		std::shared_ptr<CoreChip2D> m_coreObject;
	protected:
		std::shared_ptr<CoreChip2D> GetCoreObject() const;

	public:

		Chip2D();
		~Chip2D(){}

		void SetTexture(std::shared_ptr<Texture2D> texture);
		std::shared_ptr<Texture2D> GetTexture();

		RectF GetSrc() const;
		void SetSrc(RectF src);

		Vector2DF GetPosition() const;
		void SetPosition(Vector2DF position);

		float GetAngle() const;
		void SetAngle(float angle);

		Vector2DF GetScale() const;
		void SetScale(Vector2DF scale);

		Vector2DF GetCenterPosition() const;
		void SetCenterPosition(Vector2DF position);

		Color GetColor() const;
		void SetColor(Color color);

		bool GetTurnLR() const;
		void SetTurnLR(bool turnLR);

		bool GetTurnUL() const;
		void SetTurnUL(bool turnUL);

		int GetDrawingPriority() const;
		void SetDrawingPriority(int priority);

		AlphaBlendMode GetAlphaBlendMode() const;
		void SetAlphaBlendMode(AlphaBlendMode alphaBlend);

		void SetTextureFilterType(TextureFilterType textureFilterType);
		TextureFilterType GetTextureFilterType() const;
	};
}