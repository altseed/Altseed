using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	internal class ComponentManager<TOwner, TComponent>
		where TOwner : class
		where TComponent : Component<TOwner>
	{
		private TOwner owner { get; set; }
		private Dictionary<string, TComponent> components { get;set; }
		private Dictionary<string, TComponent> beAdded { get;set; }
		private List<string> beRemoved { get; set; }
		private bool IsUpdating { get;set; }

		public ComponentManager(TOwner owner)
		{
			this.owner = owner;
			components = new Dictionary<string, TComponent>();
			beAdded = new Dictionary<string, TComponent>();
			beRemoved = new List<string>();
			IsUpdating = false;
		}

		public void Add(TComponent component, string key)
		{
			if(component == null)
			{
				Particular.Helper.ThrowException("ArgumentNullException(Component)");
			}

			if(IsUpdating)
			{
				beAdded[key] = component;
			}
			else
			{
				components[key] = component;
			}
			component.Owner = owner;
		}

		public TComponent Get(string key)
		{
			if(components.ContainsKey(key))
			{
				return components[key];
			}
			else if(beAdded.ContainsKey(key))
			{
				return beAdded[key];
			}
			else
			{
				return null;
			}
		}

		public bool Remove(string key)
		{
			var c = Get(key);
			if(c != null)
			{
				if(IsUpdating)
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

		public void Update()
		{
			IsUpdating = true;
			var beVanished = new List<string>();
			foreach(var item in components)
			{
				item.Value.Update();
				if(!item.Value.IsAlive)
				{
					beVanished.Add(item.Key);
				}
			}
			IsUpdating = false;

			foreach(var item in beVanished)
			{
				components.Remove(item);
			}

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
