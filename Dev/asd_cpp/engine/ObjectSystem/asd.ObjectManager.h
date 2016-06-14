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
		: public std::enable_shared_from_this< ObjectManager<TObject> >
		, public IImmediateObjectManager<TObject>
	{
	private:
		typedef std::shared_ptr<TObject> ObjectPtr;

		IObjectRegisterable<TObject>* m_owner;
		std::map< int, std::list<ObjectPtr> > m_contents;

		void Redistribute(const ObjectPtr& content)
		{
			ImmediatelyRemoveObject(content, false);
			ImmediatelyAddObject(content, false);
		}

	public:
		typedef std::shared_ptr<ObjectManager<TObject> > Ptr;

		ObjectManager(IObjectRegisterable<TObject>* owner)
			: m_contents(std::map<int, std::list<ObjectPtr>>())
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

		std::list<ObjectPtr> GetContents() const
		{
			std::list<ObjectPtr> result;
			for (auto& l : m_contents)
			{
				for (auto& c : l.second)
				{
					result.push_back(c);
				}
			}
			return result;
		}

		void Add(const ObjectPtr& content)
		{
			ACE_ASSERT(content->GetLayer() == nullptr, "追加しようとしたオブジェクトは、すでに別のレイヤーに所属しています。");

			auto e = std::make_shared<EventToManageObject<TObject>>(
				this->shared_from_this(),
				content,
				RegistrationCommand::Add,
				true);
			Engine::m_changesToCommit.push(e);

			m_owner->Register(content);
		}

		void Remove(const ObjectPtr& content, bool raiseEvent)
		{
			auto e = std::make_shared<EventToManageObject<TObject>>(
				this->shared_from_this(),
				content,
				RegistrationCommand::Remove,
				raiseEvent);
			Engine::m_changesToCommit.push(e);
			m_owner->Unregister(content);
		}

		void ImmediatelyAddObject(const ObjectPtr& content, bool raiseEvent)
		{
			if (!m_owner->GetIsAlive())
			{
				return;
			}

			auto key = content->GetUpdatePriority();
			auto contents = m_contents.find(key);
			if (contents == m_contents.end())
			{
				m_contents[key] = std::list<ObjectPtr>();
				m_contents[key].push_back(content);
			}
			else
			{
				(*contents).second.push_back(content);
			}
			content->m_onUpdatePriorityChanged = [this, content](int x) { Redistribute(content); };

			m_owner->AddToCore(content);
			if (raiseEvent)
			{
				content->RaiseOnAdded();
			}
		}

		void ImmediatelyRemoveObject(const ObjectPtr& content, bool raiseEvent)
		{
			if (!m_owner->GetIsAlive())
			{
				return;
			}

			if (raiseEvent)
			{
				content->RaiseOnRemoved();
			}

			m_owner->RemoveFromCore(content);

			auto key = content->GetUpdatePriority();
			content->m_onUpdatePriorityChanged = nullptr;
			m_contents[key].remove(content);
		}

		void Clear()
		{
			for (auto& c : GetContents())
			{
				Remove(c,true); // TODO: raiseEventはtrueでよい?
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