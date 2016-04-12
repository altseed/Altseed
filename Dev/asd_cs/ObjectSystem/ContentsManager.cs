using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Mime;
using System.Text;
using System.Threading.Tasks;
using asd.Particular;

namespace asd
{
	internal class ContentsManager<TContent> where TContent : Content
	{
		SortedList<int, LinkedList<TContent>> contents_ { get; }

		public IEnumerable<TContent> Contents
		{
			get { return Lambda.ToLinear(contents_); }
		}

		public ContentsManager()
		{
			contents_ = new SortedList<int, LinkedList<TContent>>();
		}

		private void AddToContents(TContent content)
		{
			if(Lambda.HasContentHavingSpecificUpdatePriority(contents_, content.UpdatePriority))
			{
				contents_[content.UpdatePriority].AddLast(content);
			}
			else
			{
				contents_[content.UpdatePriority] = new LinkedList<TContent>(new[] { content });
			}
			content.OnUpdatePriorityChanged += Redistribute;
		}

		private bool RemoveFromContents(TContent content)
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

		public void Add(TContent content)
		{
			if(content == null)
			{
				Particular.Helper.ThrowException("ArgumentNullException(Content)");
			}

			AddToContents(content);
		}

		public bool Remove(TContent content)
		{
			return RemoveFromContents(content);
		}

		public void Clear()
		{
			foreach (var content in Contents.ToArray())
			{
				RemoveFromContents(content);
			}
		}

		public void Update()
		{
			foreach(var item in Contents.ToArray())
			{
				item.Update();
			}
		}

		public void Dispose(bool disposeNative)
		{
			foreach(var item in Contents.ToArray())
			{
				item.Dispose(disposeNative);
			}
		}
	}
}
