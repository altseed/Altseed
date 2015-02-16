
#pragma once

#include <ace.common.Base.h>
#include "ace.ComponentManager.h"

namespace ace
{
	class Object2D;

	/**
		@brief	ace::Object2D クラスに登録することができるコンポーネント。
		*/
	class Object2DComponent
	{
	public:
		typedef std::shared_ptr<Object2DComponent> Ptr;

		friend class Object2D;
		friend class ComponentManager < Object2D, Object2DComponent > ;

	private:
		Object2D* m_owner;
		bool m_isUpdated;
		bool m_isAlive;

		void Update();
		void SetOwner(Object2D* value);

	protected:
		/**
			@brief	オーバーライドして、このコンポーネントの更新処理を記述できる。
			*/
		virtual void OnUpdate() = 0;

	public:
		/**
			@brief	コンストラクタ
			*/
		Object2DComponent();
		/**
			@brief	デストラクタ
			*/
		virtual ~Object2DComponent()
		{
		}

		/**
			@brief	このインスタンスを保持している2Dオブジェクトのインスタンスを取得する。
			*/
		Object2D* GetOwner() const;

		/**
			@brief	このコンポーネントが更新されるかどうかを取得する。
			*/
		bool GetIsUpdated() const;
		/**
			@brief	このコンポーネントが更新されるかどうかを設定する。
			*/
		void SetIsUpdated(bool value);

		/**
			@brief	このコンポーネントが実行中かどうかを取得する。Vanishメソッドによって破棄された時に false を返します。
			*/
		bool GetIsAlive() const;

		/**
			@brief	このコンポーネントを破棄します。
			*/
		void Vanish();
	};
}
