#pragma once
#include <memory>
#include "ace.Object2DBase.h"

namespace ace
{
	class TextureObject2D : public Object2DBase
	{
	private:
		std::shared_ptr<CoreTextureObject2D> m_coreObject;
		std::shared_ptr<Texture2D> m_texturePtr;

		ICoreObject2D* GetCoreObject() const;

	protected:
		/**
		@brief	オーバーライドして、このオブジェクトの初期化処理を記述できる。
		*/
		virtual void OnStart();

		/**
		@brief	オーバーライドして、毎フレーム実行される更新処理を記述できる。
		*/
		virtual void OnUpdate();

		/**
		@brief	オーバーライドして、追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally();

	public:
		TextureObject2D();
		virtual ~TextureObject2D();


		std::shared_ptr<Texture2D> GetTexture() const;
		void SetTexture(std::shared_ptr<Texture2D> texture);

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

		eAlphaBlend GetAlphaBlendMode() const;
		void SetAlphaBlendMode(eAlphaBlend alphaBlend);
	};
}