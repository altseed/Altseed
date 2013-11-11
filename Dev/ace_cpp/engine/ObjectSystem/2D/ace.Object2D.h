#pragma once
#include <memory>
#include <list>
#include "../../ace.CoreToEngine.h"
#include "../Component/ace.Object2DComponent.h"

namespace ace
{
	class Layer2D;

	class Object2D
	{
		friend class Layer2D;

	public:
		typedef std::shared_ptr<Object2D> Object2DPtr;
		typedef std::shared_ptr<Object2DComponent> ComponentPtr;

	private:
		Layer2D* m_owner;
		std::list<Object2DPtr> m_children;
		std::map<astring, ComponentPtr> m_components;
		bool m_isUpdated;
		bool m_isDrawn;
		bool m_isAlive;

		void Start();
		void Update();
		void SetLayer(Layer2D* layer);
		virtual CoreObject2D* GetCoreObject() const = 0;

	protected:
		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDrawAdditionally() = 0;

	public:
		Object2D();
		virtual ~Object2D();


		bool GetIsUpdated() const;
		void SetIsUpdated(bool value);

		bool GetIsDrawn() const;
		void SetIsDrawn(bool value);

		bool GetIsAlive() const;
		void Vanish();

		/**
			@brief	このオブジェクトを保持しているレイヤーを取得する。
		*/
		Layer2D* GetLayer() const;

		void AddChild(const Object2DPtr& child, eChildMode mode);
		void RemoveChild(const Object2DPtr& child);
		const std::list<Object2DPtr>& GetChildren() const;

		void AddComponent(const ComponentPtr& component, astring key);
		ComponentPtr& GetComponent(astring key);
		void RemoveComponent(astring key);


		Vector2DF GetPosition() const;
		void SetPosition(Vector2DF position);
		Vector2DF GetGlobalPosition();

		float GetAngle() const;
		void SetAngle(float value);

		Vector2DF GetScale() const;
		void SetScale(Vector2DF value);
	};
}