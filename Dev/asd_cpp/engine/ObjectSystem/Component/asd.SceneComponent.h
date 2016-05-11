
#pragma once

#include <asd.common.Base.h>
#include "asd.ComponentManager.h"

namespace asd
{
	class Scene;

	/**
		@brief	asd::Scene クラスに登録することができるコンポーネント。
	*/
	class SceneComponent
	{
		friend class Scene;
		friend class ComponentManager<SceneComponent>;

	private:
		Scene* m_scene;
		bool m_isUpdated;
		bool m_isAlive;

		void Update();
		void SetOwner(Scene* value);

	protected:
		/**
			@brief	オーバーライドして、このコンポーネントの更新処理を記述できる。
		*/
		virtual void OnUpdate() = 0;

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
