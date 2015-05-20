
#pragma once

#include "../../../ace.Core.Base.h"
#include "../../../ace.ReferenceObject.h"

namespace ace
{
	/**
		@brief	3Dモデルクラス
	*/
	class Model
		: public IReference
	{
		friend class Accessor;
	protected:
		Model(){}
		virtual ~Model(){}

		virtual Mesh* GetMesh_(int32_t index) = 0;
		virtual AnimationClip* GetAnimationClip_(int32_t index) = 0;
	public:
		
		/**
		@brief	モデルが持つアニメーションクリップの名称を取得する。
		@param	index	アニメーションクリップのインデックス
		@return	アニメーションクリップの名称
		*/
		virtual const achar* GetAnimationClipName(int32_t index) = 0;

#if! SWIG
		/**
		@brief	モデルが持つアニメーションクリップを取得する。
		@param	index	アニメーションクリップのインデックス
		@return	アニメーションクリップ
		*/
		std::shared_ptr<AnimationClip> GetAnimationClip(int32_t index)
		{
			auto v = GetAnimationClip_(index);
			SafeAddRef(v);
			return CreateSharedPtrWithReleaseDLL(v);
		}

		/**
			@brief	メッシュを取得する。
			@param	index	メッシュのインデックス
			@return	メッシュ
		*/
		std::shared_ptr<Mesh> GetMesh(int32_t index)
		{
			auto v = GetMesh_(index);
			SafeAddRef(v);
			return CreateSharedPtrWithReleaseDLL(v);
		}
#endif
		/**
			@brief	モデルが持つメッシュの個数を取得する。
			@return	メッシュの個数
		*/
		virtual int32_t GetMeshCount() const = 0;

		/**
		@brief	モデルが持つアニメーションクリップの個数を取得する。
		@return	アニメーションクリップの個数
		*/
		virtual int32_t GetAnimationClipCount() const = 0;
	};
}