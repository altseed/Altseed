using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal class ContentsManager<TContent> where TContent : Content
	{
		SortedList<int, LinkedList<TContent>> contents_ { get; set; }
		LinkedList<TContent> beAdded { get; set; }
		LinkedList<TContent> beRemoved { get; set; }
		LinkedList<TContent> beVanished = new LinkedList<TContent>();

		bool isUpdating { get; set; }

		public IEnumerable<TContent> Contents
		{
			get { return contents_.SelectMany(x => x.Value); }
		}

		public LinkedList<TContent> VanishingContents { get { return beVanished; } }

		public ContentsManager()
		{
			contents_ = new SortedList<int, LinkedList<TContent>>();
			beAdded = new LinkedList<TContent>();
			beRemoved = new LinkedList<TContent>();
			isUpdating = false;
		}

		private void AddToContents(TContent content)
		{
			if(contents_.Any(x => x.Key == content.UpdatePriority))
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

			if(isUpdating)
			{
				beAdded.AddLast(content);
			}
			else
			{
				AddToContents(content);
			}
		}

		public bool Remove(TContent content)
		{
			if(isUpdating)
			{
				beRemoved.AddLast(content);
				return Contents.Contains(content) || beAdded.Contains(content);
			}
			else
			{
				return RemoveFromContents(content);
			}
		}

		public void Clear()
		{
			if(isUpdating)
			{
				foreach(var item in Contents)
				{
					beRemoved.AddLast(item);
				}
				beAdded.Clear();
			}
			else
			{
				contents_.Clear();
			}
		}

		public void Update()
		{
			isUpdating = true;

			foreach(var item in Contents)
			{
				item.Update();
				if(!item.GetIsAlive())
				{
					beVanished.AddLast(item);
				}
			}
			isUpdating = false;

			foreach(var item in beAdded)
			{
				AddToContents(item);
			}

			foreach(var item in beRemoved)
			{
				RemoveFromContents(item);
			}

			beAdded.Clear();
			beRemoved.Clear();
		}
	}
}
