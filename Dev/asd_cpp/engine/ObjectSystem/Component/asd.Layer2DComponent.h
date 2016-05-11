
#pragma once

#include <asd.common.Base.h>
#include "asd.ComponentManager.h"

namespace asd
{
	class Layer2D;
	template<typename TComponent> class ComponentManager;

	/**
		@brief	asd::Layer2D クラスに登録することができるコンポーネント。
	*/
	class Layer2DComponent
	{
		friend class Layer2D;
		friend class ComponentManager<Layer2DComponent>;

	private:
		Layer2D* m_layer;
		bool m_isUpdated;
		bool m_isAlive;

		void Update();
		void SetOwner(Layer2D* value);

	protected:
		/**
			@brief	オーバーライドして、このコンポーネントの更新処理を記述できる。
		*/
		virtual void OnUpdate() = 0;

	public:
		typedef std::shared_ptr<Layer2DComponent> Ptr;

		/**
			@brief	コンストラクタ
		*/
		Layer2DComponent();
		/**
			@brief	デストラクタ
		*/
		virtual ~Layer2DComponent()
		{
		}

		/**
			@brief	このインスタンスを保持しているレイヤーのインスタンスを取得する。
		*/
		Layer2D* GetOwner() const;

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
