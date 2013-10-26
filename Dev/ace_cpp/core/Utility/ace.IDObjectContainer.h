#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include <Thread/ace.CriticalSection.h>
#include <queue>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	template<typename T>
	class IDObjectContainer
	{
		static const int32_t	IndexMask = 0xfffff;
		static const int32_t	LoopMask = 0xef00000;
	private:
		std::vector<T*>	m_objects;
		std::queue<int32_t>	m_freeID;

		CriticalSection	m_section;

	public:
		void AddObject(T* o)
		{
			m_section.Enter();

			auto id = o->GetID();
			assert(id < 0);

			int32_t next = -1;
			if (m_freeID.empty())
			{
				next = (int32_t)m_objects.size();
				m_objects.push_back(o);
			}
			else
			{
				next = m_freeID.front();
				m_freeID.pop();
				auto loop = next & IndexMask;
				assert(m_objects[loop] == nullptr);
			}

			o->SetID(next);

			m_section.Leave();
		}

		void RemoveObject(T* o)
		{
			m_section.Enter();

			auto id = o->GetID();
			assert(id >= 0);

			auto loop = id & IndexMask;
		
			assert(m_objects[loop] != nullptr);
			assert(m_objects[loop] == o);
			m_objects[loop] = nullptr;

			id += (IndexMask + 1);

			m_section.Leave();
		}
	};
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
};
