
#pragma once

#include "../ace.CoreToEngine.h"

#include "ace.Layer.h"
#include "2D/ace.Layer2D.h"
#include "Component/ace.SceneComponent.h"

namespace ace
{
	class Engine;
	class Layer;

	/**
		@brief	画面を表し、レイヤーを管理するクラス。
	*/
	class Scene
	{
		friend class Engine;
		friend class Layer;

	public:
		typedef std::shared_ptr<Layer> LayerPtr;
		typedef std::shared_ptr<SceneComponent> ComponentPtr;
		typedef std::shared_ptr<Scene> Ptr;

	private:
		std::shared_ptr<CoreScene> m_coreScene;
		std::list<LayerPtr> m_layersToDraw;
		std::list<LayerPtr> m_layersToUpdate;
		std::map<astring, SceneComponent::Ptr> m_components;
		bool alreadyFirstUpdate;

		void Draw();

		void Update();
		void UpdateComponents();

		void CallTransitionFinished();
		void CallChanging();
		void CallDestroy();

	protected:
		/**
			@brief	オーバーライドして、このシーンを更新する直前の処理を記述できる。
		*/
		virtual void OnUpdating();
		/**
			@brief	オーバーライドして、このシーンを更新した直後の処理を記述できる。
		*/
		virtual void OnUpdated();
		/**
		@brief	オーバーライドして、最初のシーン更新時に実行する処理を記述する。
		*/
		virtual void OnUpdateForTheFirstTime();

		/**
		@brief	オーバーライドして、トランジション終了時に実行する処理を記述する。
		*/
		virtual void OnTransitionFinished();

		/**
		@brief	オーバーライドして、このシーンから別のシーンに切り替わる際に実行される処理を記述する。
		*/
		virtual void OnChanging();

		/**
		@brief	オーバーライドして、このシーンが無条件に破棄される際に実行される処理を記述する。
		*/
		virtual void OnDestroy();

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
		ComponentPtr& GetComponent(astring key);
		/**
			@brief	キーの示すコンポーネントをこのインスタンスから削除する。
			@param	key		削除するコンポーネントを示すキー
		*/
		void RemoveComponent(astring key);

		/**
			@brief	全てのレイヤーとポストエフェクトが描画され終わった画面をテクスチャとして取得する。
			@return	画面
			@note
			テクスチャの内容はシーンが描画されるたびに変わる。
			主にシーン遷移の際に使用する。
		*/
		std::shared_ptr<RenderTexture2D> GetEffectedScreen();
	};
}
