using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal partial class ComponentManager<TOwner, TComponent>
		where TOwner : class
		where TComponent : Component<TOwner>
	{
		private TOwner owner { get; set; }
		private Dictionary<string, TComponent> components { get; set; }
		private Dictionary<string, TComponent> beAdded { get; set; }
		private List<string> beRemoved { get; set; }
		private bool IsEnumerating { get;set; }

		public IEnumerable<TComponent> Components
		{
			get { return components.Values; }
		} 

		public ComponentManager(TOwner owner)
		{
			this.owner = owner;
			components = new Dictionary<string, TComponent>();
			beAdded = new Dictionary<string, TComponent>();
			beRemoved = new List<string>();
			IsEnumerating = false;
		}

		public void Add(TComponent component, string key)
		{
			if(component == null)
			{
				Particular.Helper.ThrowException("ArgumentNullException(Component)");
			}

			if(IsEnumerating)
			{
				Particular.Dictionary.Set(beAdded, key, component);
			}
			else
			{
				Particular.Dictionary.Set(components, key, component);
			}
			component.Owner = owner;
		}

		public bool Remove(string key)
		{
			var c = Get(key);
			if(c != null)
			{
				if(IsEnumerating)
				{
					beRemoved.Add(key);
				}
				else
				{
					components.Remove(key);
				}
				c.Owner = null;
				return true;
			}
			else
			{
				return false;
			}
		}

		public TComponent Get(string key)
		{
			if(components.ContainsKey(key))
			{
				return Particular.Dictionary.Get(components, key);
			}
			else if(beAdded.ContainsKey(key))
			{
				return Particular.Dictionary.Get(beAdded, key);
			}
			else
			{
				return null;
			}
		}

		public void StartEnumerate()
		{
			IsEnumerating = true;
		}

		public void EndEnumerate()
		{
			IsEnumerating = false;

			foreach(var item in beAdded)
			{
				components.Add(item.Key, item.Value);
			}

			foreach(var item in beRemoved)
			{
				components.Remove(item);
			}

			beAdded.Clear();
			beRemoved.Clear();
		}
	}
}
