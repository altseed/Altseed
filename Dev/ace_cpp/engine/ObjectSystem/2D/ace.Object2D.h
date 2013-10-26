#pragma once
#include <memory>
#include "../../ace.CoreToEngine.h"
#include "ace.Object2DBase.h"

namespace ace
{
	class Object2D : Object2DBase
	{
	private:
		std::shared_ptr<CoreObject2D> m_coreObject;

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
		virtual void OnDrawingAdditionally();

	public:
		Object2D();
		virtual ~Object2D();
	};
}