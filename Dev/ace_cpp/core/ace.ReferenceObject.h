#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include <memory>
#include <atomic>

#include "ace.Core.Base_Imp.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	参照カウンタオブジェクト
	*/
	class ReferenceObject
		: public IReference
	{
	private:
		mutable std::atomic<int32_t> m_reference;

	public:
		ReferenceObject();

		virtual ~ReferenceObject();

		virtual int AddRef();

		virtual int GetRef();

		virtual int Release();
	};

#if !SWIG
	/**
	@brief	全ての参照カウンタの値の合計を取得する。
	@return	参照カウンタ合計値
	@note
	実体はEngineに存在するのでそちらを参照する。
	*/
	int32_t GetGlobalReferenceCount();

	/**
	@brief	全ての参照カウンタの値を取得し、0ならDLLを解放する。
	@note
	実体はEngineに存在するのでそちらを参照する。
	*/
	void CheckDLL();

	template <typename T>
	struct ReferenceDeleter
	{
		void operator ()(T* p)
		{
			auto p_ = ((IReference*) p);
			SafeRelease(p_);
		}
	};

	template <typename T>
	struct ReferenceDeleterWithReleaseDLL
	{
		void operator ()(T* p)
		{
			auto p_ = ((IReference*) p);
			SafeRelease(p_);

			auto ref = GetGlobalReferenceCount();
			if (ref == 0)
			{
				CheckDLL();
			}
		}
	};

	template <typename T>
	static std::shared_ptr<T> CreateSharedPtr(T* p)
	{		
		return std::shared_ptr<T>(p, ReferenceDeleter<T>());
	}

	template <typename T>
	static std::shared_ptr<T> CreateSharedPtrWithReleaseDLL(T* p)
	{
		return std::shared_ptr<T>(p, ReferenceDeleterWithReleaseDLL<T>());
	}

#endif

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
};