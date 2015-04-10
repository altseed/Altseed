#pragma once
#include <memory>

namespace ace
{
	class LayerProfile
	{
	public:
		typedef std::shared_ptr<LayerProfile> Ptr;

	private:
		astring m_name;
		int m_objectCount;
		int m_time;

	public:
		LayerProfile(astring name);

		astring GetName() const;

		int GetObjectCount() const;
		void SetObjectCount(int value);

		int GetTime() const;
		void SetTime(int value);
	};
}