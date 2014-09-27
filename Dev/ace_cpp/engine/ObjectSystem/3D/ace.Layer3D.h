
#pragma once

#include "../../ace.CoreToEngine.h"
#include "../ace.Layer.h"
#include "ace.Object3D.h"

namespace ace
{
	/**
		@brief	3Dオブジェクトの更新と描画を管理するレイヤーの機能を提供するクラス
	*/
	class Layer3D
		: public Layer
	{
		friend class Scene;

	public:
		typedef std::shared_ptr<Object3D> ObjectPtr;
	
	private:

		std::shared_ptr<CoreLayer3D>	m_coreLayer;
		std::list<ObjectPtr>			m_objects;

		void BeginUpdateting();
		void EndUpdateting();

		void DrawAdditionally();
		void Update();

	protected:
		virtual void OnUpdating();
		virtual void OnUpdated();
		virtual void OnDrawAdditionally();

	public:
		/**
			@brief	コンストラクタ
		*/
		Layer3D(RenderSettings settings = RenderSettings());

		/**
			@brief	デストラクタ
		*/
		virtual ~Layer3D();

		/**
			@brief	描画設定を取得する。
			@return	描画設定
		*/
		RenderSettings GetRenderSettings() const;

		/**
			@brief	描画設定を設定する。
			@param	settings	描画設定
		*/
		void SetRenderSettings(RenderSettings settings);

		/**
			@brief	このレイヤーに指定した3Dオブジェクトを追加する。
			@param	object	追加する3Dオブジェクト
		*/
		void AddObject(const ObjectPtr& object);
		
		/**
			@brief	このレイヤーから指定した3Dオブジェクトを削除する。
			@param	object	削除される3Dオブジェクト
		*/
		void RemoveObject(const ObjectPtr& object);

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
			std::shared_ptr<Texture2D>  texture, AlphaBlend alphaBlend, bool depthWrite, bool depthTest);

		/**
			@brief	空方向の環境光の色を設定する。
			@param	color	色
		*/
		void SetSkyAmbientColor(Color color);

		/**
		@brief	地面方向の環境光の色を設定する。
		@param	color	色
		*/
		void SetGroundAmbientColor(Color color);

		/**
			@brief	テクスチャによる環境の色を設定する。
			@param	diffuseColor	拡散色
			@param	specularColor	スペキュラ色
		*/
		void SetEnvironmentColor(std::shared_ptr<CubemapTexture> diffuseColor, std::shared_ptr<CubemapTexture> specularColor);

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
			@brief	SSAOのサンプリングする半径を取得する。
			@return	SSAOのサンプリングする半径
		*/
		float GetSSAO_Radius();
		
		/**
		@brief	SSAOのサンプリングする半径を設定すする。
		@param	value	SSAOのサンプリングする半径
		*/
		void SetSSAO_Radius(float value);

		/**
		@brief	SSAOのバイアスを取得する。
		@return	SSAOのバイアス
		*/
		float GetSSAO_Bias();

		/**
		@brief	SSAOのバイアスを設定すする。
		@param	value	SSAOのバイアス
		*/
		void SetSSAO_Bias(float value);

		/**
		@brief	SSAOの強さを取得する。
		@return	SSAOの強さ
		*/
		float GetSSAO_Intensity();

		/**
		@brief	SSAOの強さを設定すする。
		@param	value	SSAOの強さ
		*/
		void SetSSAO_Intensity(float value);

		/**
		@brief	SSAOの最大距離を取得する。
		@return	SSAOの最大距離
		*/
		float GetSSAO_FarPlain();

		/**
		@brief	SSAOの最大距離を設定すする。
		@param	value	SSAOの最大距離
		*/
		void SetSSAO_FarPlain(float value);
	};
}