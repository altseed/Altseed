#pragma once

namespace asd
{
	template<typename TObject>
	class ImmediateObjectManager
	{
	protected:
		typedef std::shared_ptr<TObject> ObjectPtr;

	public:
		virtual void ImmediatelyAddObject(const ObjectPtr& object, bool raiseEvent) = 0;
		virtual void ImmediatelyRemoveObject(const ObjectPtr& object, bool raiseEvent) = 0;
	};
}