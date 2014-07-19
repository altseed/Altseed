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
		@brief	追加のテクスチャを描画する。
		*/
		void DrawSpriteAdditionally(Vector2DF pos1, Vector2DF pos2, Vector2DF pos3, Vector2DF pos4,
			Color col1, Color col2, Color col3, Color col4,
			Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, Vector2DF uv4,
			Texture2D* texture, AlphaBlend alphaBlend, int32_t priority);

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
		void RemoveComponent(astring key);


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