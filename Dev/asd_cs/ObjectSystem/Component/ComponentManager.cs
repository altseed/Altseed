using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal class ComponentManager<TOwner, TComponent>
		where TOwner : class
		where TComponent : Component<TOwner>
	{
		private TOwner owner { get; set; }
		private Dictionary<string, TComponent> components { get; set; }

		public IEnumerable<TComponent> Components
		{
			get { return components.Values; }
		}

		public ComponentManager(TOwner owner)
		{
			this.owner = owner;
			components = new Dictionary<string, TComponent>();
		}

		public void Add(TComponent component, string key)
		{
			if(component == null)
			{
				Particular.Helper.ThrowException("ArgumentNullException(Component)");
			}
			else
			{
				var e = EventToManageComponent<TOwner, TComponent>.GetAddEvent(this, component, key);
				Engine.ChangesToBeCommited.Enqueue(e);
				component.Owner = owner;
			}
		}

		public bool Remove(string key)
		{
			var c = Get(key);
			if(c != null)
			{
				var e = EventToManageComponent<TOwner, TComponent>.GetRemoveEvent(this, key);
				Engine.ChangesToBeCommited.Enqueue(e);
				c.Owner = null;
				return true;
			}
			return false;
		}

		public void AddDirectly(TComponent component, string key)
		{
			Particular.Dictionary.Set(components, key, component);
		}

		public void RemoveDirectly(string key)
		{
			components.Remove(key);
		}

		public TComponent Get(string key)
		{
			if(components.ContainsKey(key))
			{
				return Particular.Dictionary.Get(components, key);
			}
			return null;
		}
	}
}
