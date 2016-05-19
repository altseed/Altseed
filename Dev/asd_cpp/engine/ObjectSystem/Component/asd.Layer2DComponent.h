
#pragma once

#include <asd.common.Base.h>
#include <memory>
#include "asd.ComponentManager.h"
#include "../Registration/asd.IImmediatelyDisposable.h"

namespace asd
{
	class Layer2D;
	template<typename TComponent> class ComponentManager;

	/**
		@brief	asd::Layer2D クラスに登録することができるコンポーネント。
	*/
	class Layer2DComponent
		: public std::enable_shared_from_this<Layer2DComponent>
		, public IImmediatelyDisposable
	{
		friend class Layer2D;
		friend class ComponentManager<Layer2DComponent>;

	private:
		Layer2D* m_layer;
		bool m_isUpdated;
		bool m_isAlive;
		astring m_key;

		void Update();
		void SetOwner(Layer2D* value);

		void DisposeImmediately();

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

		astring GetKey() const;
		void SetKey(astring value);


		/**
			@brief	このコンポーネントが実行中かどうかを取得する。Vanishメソッドによって破棄された時に false を返します。
		*/
		bool GetIsAlive() const;

		/**
			@brief	このコンポーネントを破棄します。
		*/
		void Dispose();
	};
}
