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
		private Dictionary<string, TComponent> components_ { get;set; }
		private Dictionary<string, TComponent> componentsToBeAdded_ { get;set; }

		public ComponentManager(TOwner owner)
		{
			this.owner = owner;
			components_ = new Dictionary<string, TComponent>();
			componentsToBeAdded_ = new Dictionary<string, TComponent>();
		}

		public void Add(TComponent component, string key)
		{
			if(component == null)
			{
				throw new ArgumentNullException("component");
			}

			component.Owner = owner;
			componentsToBeAdded_[key] = component;
		}

		public TComponent Get(string key)
		{
			if(components_.ContainsKey(key))
			{
				return components_[key];
			}
			else
			{
				return componentsToBeAdded_[key];
			}
		}

		public bool Remove(string key)
		{
			if(components_.ContainsKey(key))
			{
				components_[key].Owner = null;
                components_.Remove(key);
				return true;
			}
			else if(componentsToBeAdded_.ContainsKey(key))
			{
				componentsToBeAdded_[key].Owner = null;
                componentsToBeAdded_.Remove(key);
				return true;
			}
			else
			{
				return false;
			}
		}

		public void Update()
		{
			foreach(var item in componentsToBeAdded_)
			{
				components_.Add(item.Key, item.Value);
			}
			componentsToBeAdded_.Clear();

			var beVanished = new List<string>();
			foreach(var item in components_)
			{
				item.Value.Update();
				if(!item.Value.IsAlive)
				{
					beVanished.Add(item.Key);
				}
			}

			foreach(var item in beVanished)
			{
				components_.Remove(item);
			}
		}
	}
}
