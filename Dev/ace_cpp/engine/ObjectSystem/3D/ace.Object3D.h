
#pragma once

#include "../../ace.CoreToEngine.h"

namespace ace
{
	class Layer3D;

	class Object3D
	{
		friend class Layer3D;

	public:
		typedef std::shared_ptr<Object3D> Object3DPtr;

	private:

		Layer3D*		m_owner;

		bool m_isUpdated;
		bool m_isDrawn;
		bool m_isAlive;
		
		void Start();
		void SetLayer(Layer3D* layer);
		CoreObject3D* GetCoreObject() const;

	protected:
		CoreObject3D*	m_commonObject;

	protected:
		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDrawAdditionally() = 0;

	public:
		Object3D();
		virtual ~Object3D();

		bool GetIsUpdated() const;
		void SetIsUpdated(bool value);

		bool GetIsDrawn() const;
		void SetIsDrawn(bool value);

		bool GetIsAlive() const;
		void Vanish();

		/**
		@brief	このオブジェクトを保持しているレイヤーを取得する。
		*/
		Layer3D* GetLayer() const;

		Vector3DF GetPosition() const;
	};
}