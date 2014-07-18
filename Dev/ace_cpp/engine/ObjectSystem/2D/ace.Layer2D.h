#pragma once
#include <memory>
#include <list>
#include <map>
#include "ace.Object2D.h"
#include "../../ace.CoreToEngine.h"
#include "../ace.Layer.h"
#include "../Component/ace.Layer2DComponent.h"

namespace ace
{
	/**
		@brief	2Dオブジェクトの更新と描画を管理するレイヤーの機能を提供する抽象クラス。
	*/
	class Layer2D : public Layer
	{
		friend class Scene;

	public:
		typedef std::shared_ptr<Layer2D> Ptr;

	private:
		std::shared_ptr<CoreLayer2D>	m_coreLayer;
		std::list<Object2D::Ptr>		m_objects;
		std::map<astring, Layer2DComponent::Ptr> m_components;
		std::vector<Object2D::Ptr>		beVanished;

		void BeginUpdateting();
		void EndUpdateting();

		void DrawAdditionally();
		void Update();
		void UpdateObjects();
		void UpdateComponents();

	protected:
		/**
			@brief	オーバーライドして、このレイヤーの初期化処理を記述できる。
		*/
		virtual void OnUpdating();
		/**
			@brief	オーバーライドして、このレイヤーの更新処理を記述できる。
		*/
		virtual void OnUpdated();
		/**
			@brief	オーバーライドして、このレイヤーの描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally();

	public:
		/**
			@brief	コンストラクタ
		*/
		Layer2D();
		/**
			@brief	デストラクタ
		*/
		virtual ~Layer2D();

		/**
			@brief	指定した2Dオブジェクトをこのインスタンスに追加する。
			@param	object	追加するオブジェクト
		*/
		void AddObject(const Object2D::Ptr& object);
		/**
			@brief	指定した2Dオブジェクトをこのインスタンスから削除する。
			@param	object	削除するオブジェクト
		*/
		void RemoveObject(const Object2D::Ptr& object);

		/**
			@brief	指定したコンポーネントをこのインスタンスに追加する。
			@param	component	追加するコンポーネント
			@param	key			コンポーネントに関連付けるキー
		*/
		void AddComponent(const Layer2DComponent::Ptr& component, astring key);

		/**
			@brief	指定したキーを持つコンポーネントを取得する。
			@param	key		取得するコンポーネントを示すキー
		*/
		Layer2DComponent::Ptr& GetComponent(astring key);

		/**
			@brief	指定したコンポーネントをこのインスタンスから削除する。
			@param	key		削除するコンポーネントを示すキー
		*/
		void RemoveComponent(astring key);

		/**
		@brief	追加のテクスチャを描画する。
		*/
		void DrawSpriteAdditionally(Vector2DF pos1, Vector2DF pos2, Vector2DF pos3, Vector2DF pos4,
			Color col1, Color col2, Color col3, Color col4,
			Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, Vector2DF uv4,
			Texture2D* texture, AlphaBlend alphaBlend, int32_t priority);
	};
}