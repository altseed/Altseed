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

		std::map<int, std::list<ContentPtr>> m_contents;
		std::list<ContentPtr> m_beAdded;
		std::list<ContentPtr> m_beRemoved;
		std::list<ContentPtr> beVanished;
		bool m_isUpdating;

	private:
		void AddToContents(const ContentPtr& content)
		{
			auto key = content->GetUpdatePriority();
			auto contents = m_contents.find(key);
			if (contents == m_contents.end())
			{
				m_contents[key] = std::list<ContentPtr>({ content });
			}
			else
			{
				(*contents).second.push_back(content);
			}
			content->m_onUpdatePriorityChanged = [this,content](int x) { Redistribute(content); };
		}

		void RemoveFromContents(const ContentPtr& content)
		{
			auto key = content->GetUpdatePriority();
			m_contents[key].remove(content);
		}

		void Redistribute(const ContentPtr& content)
		{
			RemoveFromContents(content);
			AddToContents(content);
		}

	public:
		ContentsManager()
			: m_contents(std::map<int, std::list<ContentPtr>>())
			, m_beAdded(std::list<ContentPtr>())
			, m_beRemoved(std::list<ContentPtr>())
			, m_isUpdating(false)
		{
		}

		std::list<ContentPtr> GetContents() const
		{
			std::list<ContentPtr> result;
			for (auto& l : m_contents)
			{
				for (auto& c : l.second)
				{
					result.push_back(c);
				}
			}
			return result;
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
				AddToContents(content);
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
				RemoveFromContents(content);
			}
		}

		void Clear()
		{
			if (m_isUpdating)
			{
				for (auto& c : GetContents())
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
			
			for (auto& list : m_contents)
			{
				for (auto& c : list.second)
				{
					c->Update();
					if (!c->GetIsAlive())
					{
						beVanished.push_back(c);
					}
				}
			}
			m_isUpdating = false;

			for (auto& c : m_beAdded)
			{
				AddToContents(c);
			}

			for (auto& c : m_beRemoved)
			{
				RemoveFromContents(c);
			}

			m_beAdded.clear();
			m_beRemoved.clear();
		}
	};
}