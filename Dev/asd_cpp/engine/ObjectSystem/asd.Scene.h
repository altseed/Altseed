
#pragma once

#include "../asd.CoreToEngine.h"

#include "asd.Layer.h"
#include "2D/asd.Layer2D.h"
#include "Component/asd.SceneComponent.h"
#include "Component/asd.ComponentManager.h"
#include "Registration/asd.IImmediatelyDisposable.h"

namespace asd
{
	class Engine;
	class EventToChangeScene;
	class EventToManageLayer;
	class Layer;
	template<typename TComponent> class ComponentManager;

	/**
		@brief	画面を表し、レイヤーを管理するクラス。
	*/
	class Scene
		: public std::enable_shared_from_this<Scene>
		, public IComponentRegisterable<SceneComponent>
		, public IImmediatelyDisposable
	{
		friend class Engine;
		friend class EventToChangeScene;
		friend class EventToManageLayer;
		friend class Layer;
		friend class SceneComponent;

	public:
		typedef std::shared_ptr<Scene> Ptr;

	private:
		std::shared_ptr<CoreScene> m_coreScene;
		std::vector<Layer::Ptr> m_layersToDraw;
		std::vector<Layer::Ptr> m_layersToUpdate;
		std::shared_ptr<ComponentManager<SceneComponent>> m_componentManager;
		bool alreadyFirstUpdate;
		bool m_isAlive;

		void RaiseOnRegistered();
		void RaiseOnStartUpdating();
		void RaiseOnTransitionFinished();
		void RaiseOnTransitionBegin();
		void RaiseOnStopUpdating();
		void RaiseOnUnregistered();

		void Update();
		void Draw();
		void CommitChanges();
		void UpdateComponents();

		void ImmediatelyAddLayer(const Layer::Ptr& layer, bool raiseEvent);
		void ImmediatelyRemoveLayer(const Layer::Ptr& layer, bool raiseEvent);
		void ImmediatelyRemoveComponent(astring key);
		void DisposeImmediately();
		void Register(const SceneComponent::Ptr& component);
		void Unregister(const SceneComponent::Ptr& component);

	protected:
		/**
		@brief	オーバーライドして、このシーンがエンジンに登録されたときに実行する処理を記述できる。
		*/
		virtual void OnRegistered();

		/**
		@brief	オーバーライドして、最初のシーン更新時に実行する処理を記述する。
		*/
		virtual void OnStartUpdating();

		/**
		@brief	オーバーライドして、トランジション終了時に実行する処理を記述する。
		*/
		virtual void OnTransitionFinished();

		/**
		@brief	オーバーライドして、このシーンから別のシーンに切り替わる際に実行される処理を記述する。
		*/
		virtual void OnTransitionBegin();

		/**
			@brief	オーバーライドして、このシーンの更新が止まるときに実行する処理を記述できる。
		*/
		virtual void OnStopUpdating();

		/**
			@brief	オーバーライドして、このシーンがエンジンから登録解除されたときに実行する処理を記述できる。
		*/
		virtual void OnUnregistered();

		/**
		@brief	オーバーライドして、このシーンが破棄される際に実行される処理を記述する。
		*/
		virtual void OnDispose();

		/**
			@brief	オーバーライドして、このシーンを更新する直前の処理を記述できる。
		*/
		virtual void OnUpdating();
		/**
			@brief	オーバーライドして、このシーンを更新した直後の処理を記述できる。
		*/
		virtual void OnUpdated();

	public:
		/**
			@brief	コンストラクタ
		*/
		Scene();
		/**
			@brief	デストラクタ
		*/
		virtual ~Scene();

		/**
			@brief	描画先のフォーマットがHDRか取得する。
			@return	HDRか?
		*/
		bool GetHDRMode() const;

		/**
			@brief	描画先のフォーマットをHDRにするか設定する。
			@param	value	HDRか?
		*/
		void SetHDRMode(bool value);

		/**
			@brief	指定したレイヤーをこのインスタンスに追加する。
			@param	layer	追加するレイヤー
		*/
		void AddLayer(const Layer::Ptr& layer);
		/**
			@brief	指定したレイヤーをこのインスタンスから削除する。
			@param	layer	削除するレイヤー
		*/
		void RemoveLayer(const Layer::Ptr& layer);

		/**
			@brief	指定したコンポーネントをこのインスタンスに追加する。
			@param	component	追加するコンポーネント
			@param	key			コンポーネントに関連付けるキー
		*/
		void AddComponent(const SceneComponent::Ptr& component, astring key);
		/**
			@brief	キーの示すコンポーネントをこのインスタンスから取得する。
			@param	key		取得するコンポーネントを示すキー
		*/
		SceneComponent::Ptr GetComponent(astring key);
		/**
			@brief	キーの示すコンポーネントをこのインスタンスから削除する。
			@param	key		削除するコンポーネントを示すキー
		*/
		bool RemoveComponent(astring key);

		/**
			@brief	全てのレイヤーとポストエフェクトが描画され終わった画面をテクスチャとして取得する。
			@return	画面
			@note
			テクスチャの内容はシーンが描画されるたびに変わる。
			主にシーン遷移の際に使用する。
		*/
		std::shared_ptr<RenderTexture2D> GetEffectedScreen();

		/**
			@brief	所属しているレイヤーを取得する。
			@return	所属しているレイヤー
		*/
		const std::vector<Layer::Ptr>& GetLayers() const;

		bool GetIsAlive() const;

		void Dispose();
	};
}
