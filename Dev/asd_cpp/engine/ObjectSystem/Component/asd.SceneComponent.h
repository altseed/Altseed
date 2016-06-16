
#pragma once

#include <asd.common.Base.h>
#include <memory>
#include "asd.ComponentManager.h"
#include "../Registration/asd.IImmediatelyDisposable.h"

namespace asd
{
	class Scene;
	template<typename TComponent> class ComponentManager;

	/**
		@brief	asd::Scene クラスに登録することができるコンポーネント。
	*/
	class SceneComponent
		: public std::enable_shared_from_this<SceneComponent>
		, public IImmediatelyDisposable
	{
		friend class Scene;
		friend class ComponentManager<SceneComponent>;

	private:
		Scene* m_scene;
		bool m_isUpdated;
		bool m_isAlive;
		astring m_key;

		void Update();
		void RaiseOnRegistered();
		void RaiseOnStartUpdating();
		void RaiseOnTransitionFinished();
		void RaiseOnTransitionBegin();
		void RaiseOnStopUpdating();
		void RaiseOnUnregistered();
		void RaiseOnDisposed();
		void SetOwner(Scene* value);

		void DisposeImmediately();

	protected:
		/**
			@brief	オーバーライドして、このコンポーネントの更新処理を記述できる。
		*/
		virtual void OnUpdate() = 0;

		virtual void OnSceneRegistered()
		{
		}

		virtual void OnStartSceneUpdating()
		{
		}

		virtual void OnSceneTransitionFinished()
		{
		}

		virtual void OnSceneTransitionBegin()
		{
		}

		virtual void OnStopSceneUpdating()
		{
		}

		virtual void OnSceneUnregistered()
		{
		}

		virtual void OnSceneDisposed()
		{
		}

	public:
		typedef std::shared_ptr<SceneComponent> Ptr;

		/**
			@brief	コンストラクタ
		*/
		SceneComponent();
		/**
			@brief	デストラクタ
		*/
		virtual ~SceneComponent()
		{
		}

		/**
			@brief	このインスタンスを保持しているシーンのインスタンスを取得する。
		*/
		Scene* GetOwner() const;

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
