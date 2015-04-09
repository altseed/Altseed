#include <memory>

namespace ace
{
	class LayerProfile
	{
	public:
		typedef std::shared_ptr<LayerProfile> Ptr;

	private:
		astring m_name;
		int m_startTime;
		int m_endTime;
		int m_objectCount;

	public:
		LayerProfile(astring name);

		astring GetName() const;

		int GetStartTime() const;
		void SetStartTime(int value);

		int GetEndTime() const;
		void SetEndTime(int value);

		int GetObjectCount() const;
		void SetObjectCount(int value);
	};
}