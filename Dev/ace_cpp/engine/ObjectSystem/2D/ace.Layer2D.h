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
			@brief	通常の描画に加えてテクスチャを描画する。
			@param	upperLeftPos	テクスチャの左上の描画位置
			@param	upperRightPos	テクスチャの右上の描画位置
			@param	lowerRightPos	テクスチャの右下の描画位置
			@param	lowerLeftPos	テクスチャの左下の描画位置
			@param	upperLeftCol	テクスチャの左上の頂点色
			@param	upperRightCol	テクスチャの右上の頂点色
			@param	lowerRightCol	テクスチャの右下の頂点色
			@param	lowerLeftCol	テクスチャの左下の頂点色
			@param	upperLeftUV		テクスチャの左上のUV値
			@param	upperRightUV	テクスチャの右上のUV値
			@param	lowerRightUV	テクスチャの右下のUV値
			@param	lowerLeftUV		テクスチャの左下のUV値
			@param	texture			描画するテクスチャ
			@param	alphaBlend		アルファブレンドの種類
			@param	priority		描画の優先順位(大きいほど前面に描画される)
			@note
			OnDrawAdditionallyの中以外では実行してはいけない。
		*/
		void DrawSpriteAdditionally(Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			std::shared_ptr<Texture2D>  texture, AlphaBlend alphaBlend, int32_t priority);
	};
}