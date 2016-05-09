#pragma once
#include <memory>
#include <list>
#include "../asd.Engine.h"
#include "Registration/asd.EventToManageObject.h"

namespace asd
{
	template<typename TOwner, typename TContent>
	class ObjectManager : public std::enable_shared_from_this<ObjectManager<TOwner, TContent>>
	{
		class EventToManageObject<TOwner, TContent>;
		friend class EventToManageObject<TOwner, TContent>;

	private:
		typedef std::shared_ptr<TContent> ContentPtr;
		typedef std::shared_ptr<TOwner> OwnerPtr;

		OwnerPtr m_owner;
		std::map<int, std::list<ContentPtr>> m_contents;
		std::list<ContentPtr> m_beAdded;
		std::list<ContentPtr> m_beRemoved;
		bool m_isUpdating;

	private:
		void AddToContents(const ContentPtr& content, bool raiseEvent)
		{
			if (!m_owner->GetIsAlive())
			{
				return;
			}

			ACE_ASSERT(content->GetLayer() == nullptr, "追加しようとしたオブジェクトは、すでに別のレイヤーに所属しています。");

			auto key = content->GetUpdatePriority();
			auto contents = m_contents.find(key);
			if (contents == m_contents.end())
			{
				m_contents[key] = std::list<ContentPtr>();
				m_contents[key].push_back(content);
			}
			else
			{
				(*contents).second.push_back(content);
			}
			content->m_onUpdatePriorityChanged = [this,content](int x) { Redistribute(content); };

			content->SetLayer(m_owner.get());
			if (raiseEvent)
			{
				content->RaiseOnAdded();
			}
		}

		void RemoveFromContents(const ContentPtr& content, bool raiseEvent)
		{
			if (!m_owner->GetIsAlive())
			{
				return;
			}

			auto key = content->GetUpdatePriority();
			content->m_onUpdatePriorityChanged = nullptr;
			m_contents[key].remove(content);

			if (raiseEvent)
			{
				content->RaiseOnRemoved();
			}
			content->SetLayer(nullptr);
		}

		void Redistribute(const ContentPtr& content)
		{
			RemoveFromContents(content, false);
			AddToContents(content, false);
		}

	public:
		ObjectManager(OwnerPtr owner)
			: m_contents(std::map<int, std::list<ContentPtr>>())
			, m_owner(owner)
			, m_beAdded(std::list<ContentPtr>())
			, m_beRemoved(std::list<ContentPtr>())
			, m_isUpdating(false)
		{
		}

		~ObjectManager()
		{
			auto contents = GetContents();
			for (auto& content : contents)
			{
				content->m_onUpdatePriorityChanged = nullptr;
			}
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

		void Add(const ContentPtr& content)
		{
			var e = std::make_shared<EventToManageObject<TOwner, TContent>>(
				shared_from_this(),
				content,
				RegistrationCommand::Add,
				true);
			Engine::m_changesToCommit.push(e);
		}

		void Remove(const ContentPtr& content, bool raiseEvent)
		{
			var e = std::make_shared<EventToManageObject<TOwner, TContent>>(
				shared_from_this(),
				content,
				RegistrationCommand::Remove,
				raiseEvent);
			Engine::m_changesToCommit.push(e);
		}

		void Clear()
		{
			for (auto& c : GetContents())
			{
				Remove(c);
			}
		}

		void Update()
		{
			for (auto& list : m_contents)
			{
				for (auto& c : list.second)
				{
					c->Update();
				}
			}
		}

		void Dispose()
		{
			for (auto& c : GetContents())
			{
				c->Dispose();
			}
		}
	};
}