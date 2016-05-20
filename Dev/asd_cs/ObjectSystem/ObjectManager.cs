using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd.Particular;

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
			get { return Lambda.ToLinear(contents_); }
		}

		public ObjectManager(IObjectRegisterable<TObject> owner)
		{
			contents_ = new SortedList<int, LinkedList<TObject>>();
			Owner = owner;
			Redistribution = new RedistributeAction<TObject>(this);
		}

		public void Add(TObject content)
		{
			if(content == null)
			{
				Particular.Helper.ThrowException("ArgumentNullException(Content)");
			}

			var e = new EventToManageObject<TObject>(this, content, RegistrationCommand.Add, true);
			Engine.ChangesToBeCommited.Enqueue(e);
		}

		public void Remove(TObject content, bool raiseEvent)
		{
			var e = new EventToManageObject<TObject>(this, content, RegistrationCommand.Remove, raiseEvent);
			Engine.ChangesToBeCommited.Enqueue(e);
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

			if(obj.IsRegisteredToLayer)
			{
				Particular.Helper.ThrowException("指定したオブジェクトは既に別のレイヤーに所属しています。");
			}

			if(!Lambda.HasContentHavingSpecificUpdatePriority(contents_, obj.UpdatePriority))
			{
				contents_[obj.UpdatePriority] = new LinkedList<TObject>();
			}
			contents_[obj.UpdatePriority].AddLast(obj);
			obj.OnUpdatePriorityChanged = Redistribution;

			Owner.Register(obj);
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

			contents_[obj.UpdatePriority].Remove(obj);
			obj.OnUpdatePriorityChanged = null;
			if(raiseEvent)
			{
				obj.RaiseOnRemoved();
			}
			Owner.Unregister(obj);
		}
	}
}


