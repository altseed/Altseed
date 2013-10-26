#pragma once
#include <memory>
#include <list>
#include "ace.Object2DBase.h"
#include "../../ace.CoreToEngine.h"
#include "../ace.Layer.h"
#include "../PostEffect/ace.PostEffect.h"

namespace ace
{
	class Layer2D : public Layer
	{
		friend class Scene;

	private:
		typedef Object2DBase Object;

		std::shared_ptr<CoreLayer2D>				m_coreLayer;
		std::list<std::shared_ptr<Object>>			m_objects;
		std::vector<std::shared_ptr<PostEffect>>	m_postEffects;
		Scene* m_scene;
		
		std::shared_ptr<CoreLayer> GetCoreLayer() const;
		void SetScene(Scene* scene);
		void Update();
		void DrawAdditionally();

		void BeginDrawing();
		void EndDrawing();

	protected:
		virtual void OnUpdating();
		virtual void OnUpdated();
		virtual void OnDrawAdditionally();

	public:
		Layer2D();
		virtual ~Layer2D();

		void AddObject(const std::shared_ptr<Object>& object);
		void RemoveObject(const std::shared_ptr<Object>& object);

		/**
			@brief	ポストエフェクトを追加する。
			@param	postEffect	ポストエフェクト
		*/
		void AddPostEffect(const std::shared_ptr<PostEffect>& postEffect);

		/**
			@brief	ポストエフェクトを全て消去する。
		*/
		void ClearPostEffects();

		int GetDrawingPriority() const;
		void SetDrawingPriority(int value);

		Scene* GetScene() const;
	};
}