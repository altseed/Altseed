﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal class RedistributeAction<TObject> : IActionWithObject
		where TObject : AltseedObject
	{
		public RedistributeAction(IImmediateObjectManager<TObject> manager)
		{
			Source = manager;
		}

		private IImmediateObjectManager<TObject> Source { get; set; }

		public void Invoke(AltseedObject obj)
		{
			Source.ImmediatelyRemoveObject((TObject)obj, false);
			Source.ImmediatelyAddObject((TObject)obj, false);
		}
	}

	internal class ObjectManager<TObject> : IImmediateObjectManager<TObject>
		where TObject : AltseedObject
	{
		private SortedList<int, LinkedList<TObject>> contents_ { get; set; }
		private IObjectRegisterable<TObject> Owner { get; set; }
		private RedistributeAction<TObject> Redistribution;

		public IEnumerable<TObject> Contents
		{
			get { return asd.Particular.Lambda.ToLinear(contents_); }
		}

		public ObjectManager(IObjectRegisterable<TObject> owner)
		{
			contents_ = new SortedList<int, LinkedList<TObject>>();
			Owner = owner;
			Redistribution = new RedistributeAction<TObject>(this);
		}

		public void Add(TObject obj)
		{
			if(obj == null)
			{
				Particular.Helper.ThrowException("ArgumentNullException(Content)");
			}

			if(obj.IsRegisteredToLayer)
			{
				Particular.Helper.ThrowException("指定したオブジェクトは既に別のレイヤーに所属しています。");
			}

			var e = new EventToManageObject<TObject>(this, obj, RegistrationCommand.Add, true);
			Engine.ChangesToBeCommited.Enqueue(e);
			Owner.__Register(obj);
		}

		public void Remove(TObject obj, bool raiseEvent)
		{
			var e = new EventToManageObject<TObject>(this, obj, RegistrationCommand.Remove, raiseEvent);
			Engine.ChangesToBeCommited.Enqueue(e);
			Owner.__Unregister(obj);
		}

		public void Clear()
		{
			foreach(var content in Contents)
			{
				Remove(content, true);
			}
		}

		public void UpdateObjects()
		{
			foreach(var item in Contents)
			{
				item.Update();
			}
		}

		public void DisposeObjects(bool disposeNative)
		{
			foreach(var item in Contents)
			{
				item.Dispose(disposeNative);
			}
		}


		public void ImmediatelyAddObject(TObject obj, bool raiseEvent)
		{
			if(!Owner.IsAlive)
			{
				return;
			}
			
			if (!asd.Particular.Lambda.HasContentHavingSpecificUpdatePriority(contents_, obj.UpdatePriority))
			{
				asd.Particular.SortedList.Set(contents_, obj.UpdatePriority, new LinkedList<TObject>());
			}
			var content = asd.Particular.SortedList.Get(contents_, obj.UpdatePriority);
			content.AddLast(obj);
			obj.OnUpdatePriorityChanged = Redistribution;

			if (!obj.IsRegisteredToLayer)
			{
				Owner.__Register(obj);
			}
			Owner.__AddToCore(obj);
			if(raiseEvent)
			{
				obj.RaiseOnAdded();
			}
		}

		public void ImmediatelyRemoveObject(TObject obj, bool raiseEvent)
		{
			if(!Owner.IsAlive || !contents_.ContainsKey(obj.UpdatePriority))
			{
				return;
			}

			if (obj.IsRegisteredToLayer)
			{
				Owner.__Unregister(obj);
			}
			if(raiseEvent)
			{
				obj.RaiseOnRemoved();
			}

			var content = asd.Particular.SortedList.Get(contents_, obj.UpdatePriority);
			content.Remove(obj);

			obj.OnUpdatePriorityChanged = null;
			Owner.__RemoveFromCore(obj);
		}
	}
}


