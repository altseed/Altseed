#pragma once

namespace asd
{
	template<typename TObject>
	class IImmediateObjectManager
	{
	protected:
		typedef std::shared_ptr<TObject> ObjectPtr;

	public:
		virtual ~IImmediateObjectManager()
		{
		}

		virtual void ImmediatelyAddObject(const ObjectPtr& object, bool raiseEvent) = 0;
		virtual void ImmediatelyRemoveObject(const ObjectPtr& object, bool raiseEvent) = 0;
	};
}