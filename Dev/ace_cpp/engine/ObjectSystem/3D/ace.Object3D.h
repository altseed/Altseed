
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

		/**
		@brief	このオブジェクトを保持しているレイヤーを取得する。
		*/
		Layer3D* GetLayer() const;

		Vector3DF GetPosition() const;
	};
}