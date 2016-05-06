using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd.Particular;

namespace asd
{
	internal class RedistributeAction<TLayer> : IActionWithObject<TLayer>
		where TLayer : Layer
	{
		public RedistributeAction(ObjectManager<TLayer> manager)
		{
			Source = manager;
		}

		private ObjectManager<TLayer> Source { get; set; }

		public void Invoke(AltseedObject<TLayer> obj)
		{
			Source.RemoveFromContents(obj, false);
			Source.AddToContents(obj, false);
		}
	}

	internal class ObjectManager<TLayer>
		where TLayer : Layer
	{
		private SortedList<int, LinkedList<AltseedObject<TLayer>>> contents_ { get; set; }
		private TLayer Owner { get; set; }
		private RedistributeAction<TLayer> Redistribution; 

		public IEnumerable<AltseedObject<TLayer>> Contents
		{
			get { return Lambda.ToLinear(contents_); }
		}

		public ObjectManager(TLayer owner)
		{
			contents_ = new SortedList<int, LinkedList<AltseedObject<TLayer>>>();
			Owner = owner;
			Redistribution = new RedistributeAction<TLayer>(this);
		}

		public void Add(AltseedObject<TLayer> @object)
		{
			if(@object == null)
			{
				Particular.Helper.ThrowException("ArgumentNullException(Content)");
			}

			var e = new EventToManageContent<TLayer>(this, @object, RegistrationCommand.Add, true);
			Engine.ChangesToBeCommited.Enqueue(e);
		}

		public void Remove(AltseedObject<TLayer> @object, bool raiseEvent)
		{
			var e = new EventToManageContent<TLayer>(this, @object, RegistrationCommand.Remove, raiseEvent);
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

		internal void AddToContents(AltseedObject<TLayer> obj, bool raiseEvent)
		{
			if (!Owner.IsAlive)
			{
				return;
			}

			if(obj.Layer != null)
			{
				Particular.Helper.ThrowException("指定したオブジェクトは既に別のレイヤーに所属しています。");
			}

			if(!Lambda.HasContentHavingSpecificUpdatePriority(contents_, obj.UpdatePriority))
			{
				contents_[obj.UpdatePriority] = new LinkedList<AltseedObject<TLayer>>();
			}
			contents_[obj.UpdatePriority].AddLast(obj);

			obj.OnUpdatePriorityChanged = Redistribution;
			obj.Layer = Owner;
			if (raiseEvent)
			{
				obj.RaiseOnAdded();
			}
		}

		internal void RemoveFromContents(AltseedObject<TLayer> obj, bool raiseEvent)
		{
			if(!Owner.IsAlive)
			{
				return;
			}

			obj.OnUpdatePriorityChanged = null;
			if(contents_.ContainsKey(obj.UpdatePriority))
			{
				contents_[obj.UpdatePriority].Remove(obj);
				if (raiseEvent)
				{
					obj.RaiseOnRemoved();
				}
				obj.Layer = null;
			}
		}
	}
}