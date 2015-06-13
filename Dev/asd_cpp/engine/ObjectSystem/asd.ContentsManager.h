#pragma once
#include <memory>
#include <list>

namespace asd
{
	template<typename TContent>
	class ContentsManager
	{
	private:
		typedef std::shared_ptr<TContent> ContentPtr;

		std::list<ContentPtr> m_contents;
		std::list<ContentPtr> m_beAdded;
		std::list<ContentPtr> m_beRemoved;
		std::list<ContentPtr> beVanished;
		bool m_isUpdating;

	public:
		ContentsManager()
			: m_contents(std::list<ContentPtr>())
			, m_beAdded(std::list<ContentPtr>())
			, m_beRemoved(std::list<ContentPtr>())
			, m_isUpdating(false)
		{
		}

		const std::list<ContentPtr>& GetContents() const
		{
			return m_contents;
		}

		std::list<ContentPtr>& GetVanishingContents()
		{
			return beVanished;
		}

		void Add(const ContentPtr& content)
		{
			if (m_isUpdating)
			{
				m_beAdded.push_back(content);
			}
			else
			{
				m_contents.push_back(content);
			}
		}

		void Remove(const ContentPtr& content)
		{
			if (m_isUpdating)
			{
				m_beRemoved.push_back(content);
			}
			else
			{
				m_contents.remove(content);
			}
		}

		void Clear()
		{
			if (m_isUpdating)
			{
				for (auto& c : m_contents)
				{
					m_beRemoved.push_back(c);
				}
				m_beAdded.clear();
			}
			else
			{
				m_contents.clear();
			}
		}

		void Update()
		{
			m_isUpdating = true;
			
			for (auto& c : m_contents)
			{
				c->Update();
				if (!c->GetIsAlive())
				{
					beVanished.push_back(c);
				}
			}
			m_isUpdating = false;

			for (auto& c : m_beAdded)
			{
				m_contents.push_back(c);
			}

			for (auto& c : m_beRemoved)
			{
				m_contents.remove(c);
			}

			m_beAdded.clear();
			m_beRemoved.clear();
		}
	};
}