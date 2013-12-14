#pragma once
#include <memory>
#include <map>
#include "../ace.CoreToEngine.h"

#include "PostEffect/ace.PostEffect.h"

namespace ace
{
	class Scene;

	/**
		@brief	オブジェクトの更新と描画を管理するレイヤーの機能を提供する抽象クラス。
	*/
	class Layer
	{
		friend class Scene;

	protected:
		std::shared_ptr<CoreLayer>	m_commonObject;
		bool m_isUpdated;
		bool m_isDrawn;

		std::vector<std::shared_ptr<PostEffect>>	m_postEffects;

	private:
		Scene* m_scene;

		virtual void SetScene(Scene* scene);

		std::shared_ptr<CoreLayer> GetCoreLayer() const;

		virtual void BeginUpdateting() = 0;
		virtual void EndUpdateting() = 0;

		virtual void Update() = 0;
		virtual void DrawAdditionally() = 0;

		void BeginDrawing();
		void EndDrawing();

	protected:
		virtual void OnUpdated() = 0;
		virtual void OnUpdating() = 0;

	public:
		Layer();
		virtual ~Layer();

		/**
			@brief	このインスタンスを管理している ace.Scene クラスのインスタンスを取得する。
			@return	シーン
		*/
		virtual Scene* GetScene() const;

		/**
			@brief	レイヤーの更新を実行するかどうか取得する。
		*/
		bool GetIsUpdated() const;

		/**
			@brief	レイヤーの更新を実行するかどうか設定する。
			@param	value	レイヤーの更新を実行するかどうか
		*/
		void SetIsUpdated(bool value);

		/**
		@brief	レイヤーを描画するどうか取得する。
		*/
		bool GetIsDrawn() const;
		/**
		@brief	レイヤーを描画するどうか設定する。
		@param	value	レイヤーを描画するどうか
		*/
		void SetIsDrawn(bool value);

		/**
		@brief	このレイヤーの描画優先度を取得する。この値が大きいほど手前に描画される。
		*/
		int GetDrawingPriority() const;

		/**
		@brief	このレイヤーの描画優先度を設定する。この値が大きいほど手前に描画される。
		@param	value	優先度
		*/
		void SetDrawingPriority(int value);

		/**
		@brief	ポストエフェクトを追加する。
		@param	postEffect	ポストエフェクト
		*/
		void AddPostEffect(const std::shared_ptr<PostEffect>& postEffect);

		/**
		@brief	ポストエフェクトを全て消去する。
		*/
		void ClearPostEffects();

	};
}
