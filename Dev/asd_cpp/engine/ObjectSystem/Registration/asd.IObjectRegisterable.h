#pragma once

namespace asd
{
	template<typename TObject>
	class IObjectRegisterable
	{
	public:
		virtual ~IObjectRegisterable()
		{
		}

		virtual bool GetIsAlive() const = 0;
		virtual void Register(const std::shared_ptr<TObject>& object) = 0;
		virtual void Unregister(const std::shared_ptr<TObject>& object) = 0;
		virtual void AddToCore(const std::shared_ptr<TObject>& object) = 0;
		virtual void RemoveFromCore(const std::shared_ptr<TObject>& object) = 0;
	};
}