
#pragma once

#include "../../ace.CoreToEngine.h"
#include "../ace.Layer.h"
#include "ace.Object3D.h"

namespace ace
{
	/**
		@brief	3Dレイヤーのクラス
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

		void BeginDrawing();
		void EndDrawing();

	protected:
		virtual void OnUpdating();
		virtual void OnUpdated();
		virtual void OnDrawAdditionally();

	public:
		/**
			@brief	コンストラクタ
		*/
		Layer3D();

		/**
			@brief	デストラクタ
		*/
		virtual ~Layer3D();

		/**
			@brief	オブジェクトを追加する。
			@param	object	オブジェクト
		*/
		void AddObject(const ObjectPtr& object);
		
		/**
			@brief	オブジェクトを除去する。
			@param	object	オブジェクト
		*/
		void RemoveObject(const ObjectPtr& object);
	};
}