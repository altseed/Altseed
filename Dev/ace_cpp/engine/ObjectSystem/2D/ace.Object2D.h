#pragma once
#include <memory>
#include <list>
#include "../../ace.CoreToEngine.h"
#include "../Component/ace.Object2DComponent.h"

namespace ace
{
	class Layer2D;

	/**
		@brief	画面上に描画される2Dオブジェクトを表すクラス。
	*/
	class Object2D
	{
		friend class Layer2D;

	public:
		typedef std::shared_ptr<Object2D> Ptr;

	private:
		Layer2D* m_owner;
		std::list<Object2D::Ptr> m_children;
		std::map<astring, Object2DComponent::Ptr> m_components;
		std::map<astring, Object2DComponent::Ptr> m_componentsToBeAdded;
		bool m_isUpdated;
		bool m_isDrawn;

		void Start();
		void Update();
		void SetLayer(Layer2D* layer);
		virtual CoreObject2D* GetCoreObject() const = 0;

		void UpdateComponents();

	protected:
		/**
			@brief	オーバーライドして、このオブジェクトの初期化処理を記述できる。
		*/
		virtual void OnStart();
		/**
			@brief	オーバーライドして、このオブジェクトの更新処理を記述できる。
		*/
		virtual void OnUpdate();
		/**
			@brief	オーバーライドして、このオブジェクトに対する追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally();

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
			std::shared_ptr<Texture2D> texture, AlphaBlend alphaBlend, int32_t priority);

		/**
		@brief	通常の描画に加えて文字列を描画する。
		@param	pos					描画位置
		@param	color				頂点色
		@param	font				フォント
		@param	text				文字列
		@param	writingDirection	行方向
		@param	alphaBlend		アルファブレンドの種類
		@param	priority		描画の優先順位(大きいほど前面に描画される)
		@note
		OnDrawAdditionallyの中以外では実行してはいけない。
		*/
		void DrawTextAdditionally(Vector2DF pos, Color color, std::shared_ptr<Font> font, const achar* text, WritingDirection writingDirection, AlphaBlend alphaBlend, int32_t priority);

	public:
		/**
			@brief	コンストラクタ
		*/
		Object2D();
		/**
			@brief	デストラクタ
		*/
		virtual ~Object2D();

		/**
			@brief	このオブジェクトが更新されるかどうかを取得する。
		*/
		bool GetIsUpdated() const;
		/**
			@brief	このオブジェクトが更新されるかどうかを設定する。
		*/
		void SetIsUpdated(bool value);

		/**
			@brief	このオブジェクトが描画されるかどうかを取得する。
		*/
		bool GetIsDrawn() const;

		/**
			@brief	このオブジェクトが描画されるかどうかを設定する。
		*/
		void SetIsDrawn(bool value);

		/**
			@brief	このオブジェクトが実行中かどうかを取得する。Vanishメソッドによって破棄された時に false を返す。
		*/
		bool GetIsAlive() const;

		/**
			@brief	このオブジェクトを破棄する。
		*/
		void Vanish();

		/**
			@brief	このオブジェクトを保持しているレイヤーを取得する。
		*/
		Layer2D* GetLayer() const;

		/**
			@brief	指定した2Dオブジェクトを子オブジェクトとしてこのインスタンスに追加する。
			@param	child	追加する子オブジェクト
			@param	mode	子オブジェクトの同期モード
		*/
		void AddChild(const Object2D::Ptr& child, eChildMode mode);
		/**
			@brief	指定した子オブジェクトをこのインスタンスから削除する。
			@param	child	削除する子オブジェクト
		*/
		void RemoveChild(const Object2D::Ptr& child);
		/**
			@brief	このオブジェクトが保持している子オブジェクトを含むコンテナを取得する。
		*/
		const std::list<Object2D::Ptr>& GetChildren() const;

		/**
			@brief	指定したコンポーネントをこのインスタンスに追加する。
			@param	component	追加するコンポーネント
			@param	key			コンポーネントに関連付けるキー
		*/
		void AddComponent(const Object2DComponent::Ptr& component, astring key);
		/**
			@brief	指定したキーを持つコンポーネントを取得する。
			@param	key		取得するコンポーネントを示すキー
		*/
		Object2DComponent::Ptr& GetComponent(astring key);
		/**
			@brief	指定したコンポーネントを削除する。
			@param	key		削除するコンポーネントを示すキー
		*/
		bool RemoveComponent(astring key);


		/**
			@brief	このオブジェクトを描画する位置を取得する。
		*/
		Vector2DF GetPosition() const;
		/**
			@brief	このオブジェクトを描画する位置を設定する。
		*/
		void SetPosition(Vector2DF position);
		/**
			@brief	このオブジェクトを描画する実際の位置を取得する。親子関係がある場合に、親の位置を考慮した値を取得できる。
		*/
		Vector2DF GetGlobalPosition();

		/**
			@brief	このオブジェクトを描画する際の回転角度を取得する。
		*/
		float GetAngle() const;
		/**
			@brief	このオブジェクトを描画する際の回転角度を設定する。
		*/
		void SetAngle(float value);

		/**
			@brief	このオブジェクトを描画する際の拡大率を取得する。
		*/
		Vector2DF GetScale() const;
		/**
			@brief	このオブジェクトを描画する際の拡大率を設定する。
		*/
		void SetScale(Vector2DF value);
	};
}