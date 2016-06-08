using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal class ComponentManager<TComponent> : IImmediateComponentmanager<TComponent>
		where TComponent : Component
	{
		private IComponentRegisterable<TComponent> owner { get; set; }
		private Dictionary<string, TComponent> components { get; set; }

		public IEnumerable<TComponent> Components
		{
			get { return components.Values; }
		}

		public ComponentManager(IComponentRegisterable<TComponent> owner)
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
				var e = new EventToManageComponent<TComponent>();
				e.SetUpAsAddEvent(this, component, key);
				Engine.ChangesToBeCommited.Enqueue(e);
			}
		}

		public bool Remove(string key)
		{
			var c = Get(key);
			if(c != null)
			{
				var e = new EventToManageComponent<TComponent>();
				e.SetUpAsRemoveEvent(this, key);
				Engine.ChangesToBeCommited.Enqueue(e);
				return true;
			}
			return false;
		}

		public void ImmediatelyAddComponent(TComponent component, string key)
		{
			Particular.Dictionary.Set(components, key, component);
			owner.__Register(component);
			component.Key = key;
		}

		public void ImmediatelyRemoveComponent(string key)
		{
			var component = Get(key);
			components.Remove(key);
			owner.__Unregister(component);
			component.Key = null;
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
