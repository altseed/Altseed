#pragma once
#include <memory>
#include <list>
#include "ace.Object2DBase.h"
#include "../../ace.CoreToEngine.h"
#include "../ace.Layer.h"
#include "../PostEffect/ace.PostEffect.h"
#include "../Component/ace.LayerComponent.h"

namespace ace
{
	class Layer2D : public Layer
	{
		friend class Scene;

	public:
		typedef std::shared_ptr<Object2DBase> ObjectPtr;

	private:
		std::shared_ptr<CoreLayer2D>	m_coreLayer;
		std::list<ObjectPtr>			m_objects;
		std::vector<std::shared_ptr<PostEffect>>	m_postEffects;
		
		std::shared_ptr<CoreLayer> GetCoreLayer() const;
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

		void AddObject(const ObjectPtr& object);
		void RemoveObject(const ObjectPtr& object);

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
	};
}