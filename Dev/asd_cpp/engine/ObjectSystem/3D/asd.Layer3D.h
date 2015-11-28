
#pragma once

#include "../../asd.CoreToEngine.h"
#include "../asd.Layer.h"
#include "asd.Object3D.h"

namespace asd
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
		ContentsManager<Object3D>		m_objects;

		void BeginUpdating();
		void EndUpdateting();

		void DrawAdditionally();
		void UpdateInternal();
		void Dispose();

	protected:

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
		void AddObject(const Object3D::Ptr& object);

		/**
			@brief	このレイヤーから指定した3Dオブジェクトを削除する。
			@param	object	削除される3Dオブジェクト
			*/
		void RemoveObject(const Object3D::Ptr& object);
		/**
		@brief	このレイヤーに登録されている3Dオブジェクトのリストを取得する。
		@return	登録されているオブジェクトのリスト
		*/
		const std::list<Object3D::Ptr>& GetObjects() const;

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

		/**
		@brief	環境光の強さを取得する。
		@return	環境光の強さ
		@note
		デフォルトは1.0である。
		*/
		float GetAmbientColorIntensity();

		/**
		@brief	環境光の強さを設定する。
		@param	ambientColorIntensity	環境光の強さ
		@note
		デフォルトは1.0である。
		*/
		void SetAmbientColorIntensity(float ambientColorIntensity);

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
		@brief	テクスチャによる環境の色の強さを設定する。
		@param	environmentDiffuseColorIntensity	拡散色の強さ
		@param	environmentSpecularColorIntensity	スペキュラ色の強さ
		@note
		デフォルトは1.0である。
		*/
		void SetEnvironmentColorIntensity(float environmentDiffuseColorIntensity, float environmentSpecularColorIntensity);

		/**
			@brief	テクスチャによる環境の色を設定する。
			@param	diffuseColor	拡散色
			@param	specularColor	スペキュラ色
			*/
		void SetEnvironmentColor(std::shared_ptr<CubemapTexture> diffuseColor, std::shared_ptr<CubemapTexture> specularColor);

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

		/**
		@brief	高さ方向のフォグが有効かどうか、を取得する。
		*/
		bool GetIsHeightFogEnabled();
		
		/**
		@brief	高さ方向のフォグが有効かどうか、を設定する。
		*/
		void SetIsHeightFogEnabled(bool value);

		/**
		@brief	高さ方向のフォグの濃度を取得する。
		*/
		float GetHeightFogDensity();
		
		/**
		@brief	高さ方向のフォグの濃度を設定する。
		*/
		void SetHeightFogDensity(float value);

		/**
		@brief	高さ方向のフォグの色を取得する。
		*/
		Color GetHeightFogColor();

		/**
		@brief	高さ方向のフォグの色を設定する。
		*/
		void SetHeightFogColor(Color value);

		/**
		@brief	高さ方向のフォグの減衰率を取得する。
		@note
		この値が大きいほど、高い位置のフォグが薄くなる。
		*/
		float GetHeightFogFalloff();
		
		/**
		@brief	高さ方向のフォグの減衰率を設定する。
		*/
		void SetHeightFogFalloff(float value);

		/**
		@brief	高さ方向のフォグの開始距離を取得する。
		@note
		この値よりカメラに対して近い位置には霧が生じなくなる。
		*/
		float GetHeightFogStartDistance();

		/**
		@brief	高さ方向のフォグの開始距離を設定する。
		*/
		void SetHeightFogStartDistance(float value);

		bool GetIsOceanEnabled();
		void SetIsOceanEnabled(bool value);
		RectF GetOceanArea();
		void SetOceanArea(RectF value);
		float GetOceanHeight();
		void SetOceanHeight(float value);
		float GetOceanGridSize();
		void SetOceanGridSize(float value);
		Color GetOceanColor();
		void SetOceanColor(Color color);
		float GetOceanDensity();
		void SetOceanDensity(float value);
		Texture2D* GetOceanNormalMap();
		void SetOceanNormalMap(std::shared_ptr<Texture2D> value);

		/**
		@brief	レイヤーの種類を取得する。
		@return	レイヤーの種類
		*/
		LayerType GetLayerType() const override { return LayerType::Layer3D; }

		int GetObjectCount() const;
	};
}