#pragma once
#include <memory>
#include <list>
#include <map>
#include "ace.Object2D.h"
#include "../../ace.CoreToEngine.h"
#include "../ace.Layer.h"
#include "../PostEffect/ace.PostEffect.h"
#include "../Component/ace.Layer2DComponent.h"

namespace ace
{
	class Layer2D : public Layer
	{
		friend class Scene;

	public:
		typedef std::shared_ptr<Object2D> ObjectPtr;
		typedef std::shared_ptr<Layer2DComponent> ComponentPtr;

	private:
		std::shared_ptr<CoreLayer2D>	m_coreLayer;
		std::list<ObjectPtr>			m_objects;
		std::map<astring, ComponentPtr> m_components;
		std::vector<std::shared_ptr<PostEffect>>	m_postEffects;
		bool m_isUpdated;
		bool m_isDrawn;
		
		std::shared_ptr<CoreLayer> GetCoreLayer() const;
		void DrawAdditionally();
		void Update();

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

		void AddComponent(const ComponentPtr& component, astring key);
		ComponentPtr& GetComponent(astring key);
		void RemoveComponent(astring key);

		int GetDrawingPriority() const;
		void SetDrawingPriority(int value);

		bool GetIsUpdated() const;
		void SetIsUpdated(bool value);

		bool GetIsDrawn() const;
		void SetIsDrawn(bool value);
	};
}