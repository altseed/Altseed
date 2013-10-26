#pragma once
#include <list>
#include <memory>
#include "ace.CoreScene.h"

namespace ace
{
	class CoreScene_Imp 
		: public CoreScene
		, public ReferenceObject
	{
		friend class ObjectSystemFactory_Imp;
	private:
		std::list<LayerPtr> m_layers;

		CoreScene_Imp();
		virtual ~CoreScene_Imp();
	public:
		
		void AddLayer(LayerPtr layer);
		void RemoveLayer(LayerPtr layer);
		
		void Draw();

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}