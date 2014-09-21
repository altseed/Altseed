
#pragma once

#include "../../ace.CoreToEngine.h"

namespace ace
{
	class Layer3D;

	/**
		@brief	更新・描画処理を行う単位となる3Dオブジェクトの機能を提供するクラス
	*/
	class Object3D
	{
		friend class Layer3D;

	public:
		typedef std::shared_ptr<Object3D> Object3DPtr;

	private:

		Layer3D*		m_owner;

		bool m_isUpdated;
		bool m_isDrawn;
		bool m_isAlive;
		
		void Start();
		void Update();
		void SetLayer(Layer3D* layer);
		CoreObject3D* GetCoreObject() const;

	protected:
		CoreObject3D*	m_commonObject;

		/**
		@brief	ユーザーはオーバーライドしてはいけない。
		*/
		virtual void OnUpdateInternal() {}

	protected:

		/**
			@brief	オーバーライドして、この3Dオブジェクトの初期化処理を記述することができる。
		*/
		virtual void OnStart() = 0;

		/**
			@brief	オーバーライドして、この2Dオブジェクトの更新処理を記述することができる。
		*/
		virtual void OnUpdate() = 0;

		/**
			@brief	オーバーライドして、この2Dオブジェクトに関する追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally() = 0;

	public:
		/**
			@brief	コンストラクタ
		*/
		Object3D();

		/**
			@brief	デストラクタ
		*/
		virtual ~Object3D();

		/**
		@brief	オブジェクトの更新を実行するかどうか取得する。
		*/
		bool GetIsUpdated() const;

		/**
		@brief	オブジェクトの更新を実行するかどうか設定する。
		@param	value	オブジェクトの更新を実行するかどうか
		*/
		void SetIsUpdated(bool value);

		/**
		@brief	オブジェクトを描画するどうか取得する。
		*/
		bool GetIsDrawn() const;
		/**
		@brief	オブジェクトを描画するどうか設定する。
		@param	value	オブジェクトを描画するどうか
		*/
		void SetIsDrawn(bool value);

		/**
		@brief	オブジェクトが生存しているか取得する。
		@return	生存しているか
		*/
		bool GetIsAlive() const;

		/**
		@brief	オブジェクトを破棄する。
		*/
		void Vanish();

		/**
		@brief	このインスタンスを管理している ace.Layer3D クラスのインスタンスを取得する。
		*/
		Layer3D* GetLayer() const;

		/**
			@brief	このインスタンスの親に対する現在の座標を取得する。
			@return	座標
		*/
		Vector3DF GetPosition() const;

		/**
			@brief	このインスタンスの親に対する現在の座標を設定する。
			@param	position	座標
		*/
		void SetPosition(Vector3DF position);

		/**
		@brief	このインスタンスの親に対する現在の角度を取得する。
		@return	角度
		*/
		Vector3DF GetRotation() const;

		/**
		@brief	このインスタンスの親に対する現在の角度を設定する。
		@param	angle	角度
		*/
		void SetRotation(Vector3DF angle);

		/**
		@brief	このインスタンスの親に対する現在の拡大率を取得する。
		@return	拡大率
		*/
		Vector3DF GetScale() const;

		/**
		@brief	このインスタンスの親に対する現在の拡大率を設定する。
		@param	scale	拡大率
		*/
		void SetScale(Vector3DF scale);
	};
}