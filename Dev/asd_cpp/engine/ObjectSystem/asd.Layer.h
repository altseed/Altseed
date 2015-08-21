#pragma once
#include <memory>
#include <map>
#include "../asd.CoreToEngine.h"

#include "PostEffect/asd.PostEffect.h"

namespace asd
{
	class Engine;
	class Scene;

	/**
		@brief	オブジェクトの更新と描画を管理するレイヤーの機能を提供する抽象クラス。
	*/
	class Layer
	{
		friend class Scene;
	public:
		typedef std::shared_ptr<Layer> Ptr;

	protected:
		std::shared_ptr<CoreLayer>	m_commonObject;
		bool m_isUpdated;
		bool m_isUpdatedCurrent = false;
		bool m_isDrawn;
		bool m_isAlive;
		int m_updatePriority;

		std::vector<std::shared_ptr<PostEffect>>	m_postEffects;

	private:
		Scene* m_scene;
		astring m_name;

		virtual void SetScene(Scene* scene);

		std::shared_ptr<CoreLayer> GetCoreLayer() const;

		virtual void BeginUpdating() = 0;
		virtual void EndUpdateting() = 0;

		virtual void Update() = 0;
		virtual void DrawAdditionally() = 0;
		virtual void CallDestroy() = 0;

		void BeginDrawing();
		void EndDrawing();
		void Start();
		void Draw();

	protected:

		/**
		@brief	オーバーライドして、このレイヤーの初期化処理を記述できる。
		*/
		virtual void OnStart();
		
		/**
		@brief	オーバーライドして、このレイヤーが更新される前の処理を記述できる。
		*/
		virtual void OnUpdating();
		
		/**
		@brief	オーバーライドして、このレイヤーが更新された後の処理を記述できる。
		*/
		virtual void OnUpdated();
	
		/**
		@brief	オーバーライドして、このレイヤーの描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally();

		/**
		@brief	オーバーライドして、このレイヤーがVanishメソッドによって破棄されるときの処理を記述できる。
		*/
		virtual void OnVanish();
		
		/**
		@brief	オーバーライドして、このレイヤーが破棄されるときの処理を記述できる。
		*/
		virtual void OnDispose();

	public:
		Layer();
		virtual ~Layer();

		/**
			@brief	このインスタンスを管理している asd.Scene クラスのインスタンスを取得する。
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
			@brief	このレイヤーが有効かどうかを取得する。
		*/
		bool GetIsAlive() const;

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

		/**
			@brief	このレイヤーを破棄する。
		*/
		void Vanish();

		/**
			@brief	レイヤーの種類を取得する。
			@return	レイヤーの種類
		*/
		virtual LayerType GetLayerType() const = 0;

		/**
			@brief	このレイヤーの前回の更新時間を取得する。
		*/
		int GetTimeForUpdate() const;

		virtual int GetObjectCount() const = 0;

		/**
			@brief	このレイヤーの名前を取得する。
		*/
		astring GetName() const;
		/**
			@brief	このレイヤーの名前を設定する。
		*/
		void SetName(astring value);

		/**
			@brief	このレイヤーの更新の優先順位を取得する。
		*/
		int GetUpdatePriority() const;
		/**
			@brief	このレイヤーの更新の優先順位を設定する。
		*/
		void SetUpdatePriority(int value);

	};
}
