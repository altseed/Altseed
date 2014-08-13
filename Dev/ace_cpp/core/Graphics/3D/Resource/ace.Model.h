
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
	public:
		

#if! SWIG
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
	};
}