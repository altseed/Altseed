using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	internal class ContentsManager<TContent> where TContent : Content
	{
		LinkedList<TContent> contents_ { get; set; }
		LinkedList<TContent> beAdded { get; set; }
		LinkedList<TContent> beRemoved { get; set; }
		bool isUpdating { get; set; }

		public IEnumerable<TContent> Contents
		{
			get { return contents_; }
		}

		public ContentsManager()
		{
			contents_ = new LinkedList<TContent>();
			beAdded = new LinkedList<TContent>();
			beRemoved = new LinkedList<TContent>();
		}

		public void Add(TContent content)
		{
			if(content == null)
			{
				throw new ArgumentNullException("content");
			}

			if(isUpdating)
			{
				beAdded.AddLast(content);
			}
			else
			{
				contents_.AddLast(content);
			}
		}

		public bool Remove(TContent content)
		{
			if(isUpdating)
			{
				beRemoved.AddLast(content);
				return contents_.Contains(content) || beAdded.Contains(content);
			}
			else
			{
				return contents_.Remove(content);
			}
		}

		public void Clear()
		{
			if(isUpdating)
			{
				foreach(var item in contents_)
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
			var beVanished = new LinkedList<TContent>();
			foreach(var item in contents_)
			{
				item.Update();
				if(!item.GetIsAlive())
				{
					beVanished.AddLast(item);
				}
			}
			isUpdating = false;

			foreach(var item in beVanished)
			{
				contents_.Remove(item);
			}

			foreach(var item in beAdded)
			{
				contents_.AddLast(item);
			}

			foreach(var item in beRemoved)
			{
				contents_.Remove(item);
			}

			beAdded.Clear();
			beRemoved.Clear();
		}
	}
}
