
#pragma once

#include <asd.common.Base.h>
#include <memory>
#include "asd.ComponentManager.h"
#include "../Registration/asd.IImmediatelyDisposable.h"

namespace asd
{
	class Object2D;
	template<typename TComponent> class ComponentManager;

	/**
		@brief	asd::Object2D クラスに登録することができるコンポーネント。
	*/
	class Object2DComponent
		: public IImmediatelyDisposable
		, public std::enable_shared_from_this<Object2DComponent>
	{
		friend class Object2D;
		friend class ComponentManager<Object2DComponent>;

	private:
		Object2D* m_owner;
		bool m_isUpdated;
		bool m_isAlive;
		astring m_key;

		void Update();
		void RaiseOnAdded();
		void RaiseOnRemoved();
		void RaiseOnDisposed();
		void SetOwner(Object2D* value);

		void DisposeImmediately();

	protected:
		/**
			@brief	オーバーライドして、このコンポーネントの更新処理を記述できる。
			*/
		virtual void OnUpdate() = 0;

		virtual void OnObjectAdded()
		{
		}

		virtual void OnObjectRemoved()
		{
		}

		virtual void OnObjectDisposed()
		{
		}

	public:
		typedef std::shared_ptr<Object2DComponent> Ptr;

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
