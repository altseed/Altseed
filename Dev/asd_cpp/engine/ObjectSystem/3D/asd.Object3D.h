
#pragma once

#include <functional>
#include "../../asd.CoreToEngine.h"
#include "../asd.ContentsManager.h"

namespace asd
{
	class Layer3D;

	/**
		@brief	更新・描画処理を行う単位となる3Dオブジェクトの機能を提供するクラス
	*/
	class Object3D
	{
		friend class Layer3D;
		friend class ContentsManager < Object3D > ;

	public:
		typedef std::shared_ptr<Object3D> Ptr;

	private:

		Layer3D*		m_owner;

		bool m_isUpdated;
		bool m_isDrawn;
		bool m_isAlive;
		int m_updatePriority;
		std::function<void(int)> m_onUpdatePriorityChanged;
		
		void RaiseOnAdded();
		void RaiseOnRemoved();
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
			@brief	オーバーライドして、この3Dオブジェクトがレイヤーに登録されたときの処理を記述できる。
		*/
		virtual void OnAdded();

		/**
			@brief	オーバーライドして、この3Dオブジェクトがレイヤーから登録解除されたときの処理を記述できる。
		*/
		virtual void OnRemoved();

		/**
			@brief	オーバーライドして、この3Dオブジェクトが破棄されるときの処理を記述できる。
		*/
		virtual void OnDispose();

		/**
			@brief	オーバーライドして、この3Dオブジェクトの更新処理を記述することができる。
		*/
		virtual void OnUpdate() = 0;

		/**
			@brief	オーバーライドして、この3Dオブジェクトに関する追加の描画処理を記述できる。
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
		void Dispose();

		/**
		@brief	このインスタンスを管理している asd.Layer3D クラスのインスタンスを取得する。
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

		/**
		@brief	モデル行列を直接取得する。
		@return	モデル行列
		*/
		Matrix44 GetLocalMatrix();

		/**
		@brief	モデル行列を直接設定する。
		@param	localMat	モデル行列
		@note
		この値を設定した場合、通常の移動や回転は無効化される。
		*/
		void SetLoalMatrixDirectly(Matrix44 localMat);

		/**
			@brief	このインスタンスの更新の優先順位を取得する。
		*/
		int GetUpdatePriority() const;
		/**
			@brief	このインスタンスの更新の優先順位を設定する。
		*/
		void SetUpdatePriority(int value);


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
		@param	depthWrite		深度を書き込むか?
		@param	depthTest		深度を比較するか?
		@note
		OnDrawAdditionallyの中以外では実行してはいけない。
		*/
		void DrawSpriteAdditionally(Vector3DF upperLeftPos, Vector3DF upperRightPos, Vector3DF lowerRightPos, Vector3DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			std::shared_ptr<Texture2D>  texture, AlphaBlendMode alphaBlend, bool depthWrite, bool depthTest);
	};
}