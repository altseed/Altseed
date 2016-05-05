using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd.Particular;

namespace asd
{
	internal class ContentsManager<TContent> where TContent : Content
	{
		private SortedList<int, LinkedList<TContent>> contents_ { get; set; }
		private TContent[] ActualContents { get; set; }

		public IEnumerable<TContent> Contents
		{
			get { return Engine.RegistrationManager.GetActualContents(this, contents_.SelectMany(x => x.Value)); }
		}

		public ContentsManager()
		{
			contents_ = new SortedList<int, LinkedList<TContent>>();
		}

		public void Add(TContent content)
		{
			if(content == null)
			{
				Particular.Helper.ThrowException("ArgumentNullException(Content)");
			}

			Engine.RegistrationManager.Push(new EventToManageContent<TContent>(this, content, RegistrationCommand.Add));
		}

		public bool Remove(TContent content)
		{
			var result = contents_.SelectMany(x => x.Value).Contains(content)
				| Engine.RegistrationManager.Push(new EventToManageContent<TContent>(this, content, RegistrationCommand.Remove));

			return (bool) result;
		}

		public void Clear()
		{
			foreach(var content in Contents)
			{
				Remove(content);
			}
		}

		public void Update()
		{
			foreach(var item in Contents)
			{
				item.Update();
			}
		}

		public void Dispose(bool disposeNative)
		{
			foreach(var item in Contents)
			{
				item.Dispose(disposeNative);
			}
		}

		internal void AddToContents(TContent content)
		{
			if(!Lambda.HasContentHavingSpecificUpdatePriority(contents_, content.UpdatePriority))
			{
				contents_[content.UpdatePriority] = new LinkedList<TContent>();
			}
			contents_[content.UpdatePriority].AddLast(content);
			content.OnUpdatePriorityChanged += Redistribute;
		}

		internal bool RemoveFromContents(TContent content)
		{
			content.OnUpdatePriorityChanged -= Redistribute;
			if(contents_.ContainsKey(content.UpdatePriority))
			{
				return contents_[content.UpdatePriority].Remove(content);
			}
			return false;
		}

		private void Redistribute(Content content)
		{
			RemoveFromContents((TContent)content);
			AddToContents((TContent)content);
		}
	}
}