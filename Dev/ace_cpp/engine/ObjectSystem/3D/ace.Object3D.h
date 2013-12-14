
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

	protected:
		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;
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

		bool GetIsAlive() const;
		void Vanish();

		/**
		@brief	このインスタンスを管理している ace.Layer3D クラスのインスタンスを取得する。
		*/
		Layer3D* GetLayer() const;

		Vector3DF GetPosition() const;
		void SetPosition(Vector3DF position);

		Vector3DF GetRotation() const;
		void SetRotation(Vector3DF angle);
	};
}