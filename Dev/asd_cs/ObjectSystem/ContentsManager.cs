using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal class ContentsManager<TContent> where TContent : Content
	{
		LinkedList<TContent> contents_ { get; set; }
		LinkedList<TContent> beAdded { get; set; }
		LinkedList<TContent> beRemoved { get; set; }
		LinkedList<TContent> beVanished = new LinkedList<TContent>();

		bool isUpdating { get; set; }

		public IEnumerable<TContent> Contents
		{
			get { return contents_; }
		}

		public LinkedList<TContent> VanishingContents { get { return beVanished; } }

		public ContentsManager()
		{
			contents_ = new LinkedList<TContent>();
			beAdded = new LinkedList<TContent>();
			beRemoved = new LinkedList<TContent>();
			isUpdating = false;
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
			
			foreach(var item in contents_)
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
