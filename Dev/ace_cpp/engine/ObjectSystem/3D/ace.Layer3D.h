
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
	};
}