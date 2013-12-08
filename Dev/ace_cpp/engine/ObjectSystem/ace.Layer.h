#pragma once
#include <memory>
#include <map>
#include "../ace.CoreToEngine.h"

#include "PostEffect/ace.PostEffect.h"

namespace ace
{
	class Scene;

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

		virtual Scene* GetScene() const;

		bool GetIsUpdated() const;
		void SetIsUpdated(bool value);

		bool GetIsDrawn() const;
		void SetIsDrawn(bool value);

		int GetDrawingPriority() const;
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
