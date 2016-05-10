#pragma once
#include <memory>
#include <list>
#include "../asd.Engine.h"
#include "Registration/asd.EventToManageObject.h"
#include "Registration/asd.IImmediateObjectManager.h"
#include "Registration/asd.IObjectRegisterable.h"

namespace asd
{
	template<typename TObject>
	class ObjectManager
		: public std::enable_shared_from_this<ObjectManager<TObject>>
		, public ImmediateObjectManager<TObject>
	{
		friend class EventToManageObject<TObject>;

	private:
		typedef std::shared_ptr<TObject> ObjectPtr;

		IObjectRegisterable<TObject>* m_owner;
		std::map<int, std::list<ObjectPtr>> m_contents;

	private:
		void ImmediatelyAddObject(const ContentPtr& content, bool raiseEvent)
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

			m_owner->Register(content);
			if (raiseEvent)
			{
				content->RaiseOnAdded();
			}
		}

		void ImmediatelyRemoveObject(const ContentPtr& content, bool raiseEvent)
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
			m_owner->Unregister(content);
		}

		void Redistribute(const ContentPtr& content)
		{
			RemoveFromContents(content, false);
			AddToContents(content, false);
		}

	public:
		ObjectManager(IObjectRegisterable<TObject>* owner)
			: m_contents(std::map<int, std::list<ContentPtr>>())
			, m_owner(owner)
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
			var e = std::make_shared<EventToManageObject<ObjectPtr>>(
				shared_from_this(),
				content,
				RegistrationCommand::Add,
				true);
			Engine::m_changesToCommit.push(e);
		}

		void Remove(const ContentPtr& content, bool raiseEvent)
		{
			var e = std::make_shared<EventToManageObject<ObjectPtr>>(
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