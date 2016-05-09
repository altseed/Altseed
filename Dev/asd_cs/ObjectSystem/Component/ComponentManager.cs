using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd.ObjectSystem.Registration;

namespace asd
{
	internal class ComponentManager<TComponent> : IImmediatelyComponentmanager<TComponent>
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
				var e = EventToManageComponent<TComponent>.GetAddEvent(this, component, key);
				Engine.ChangesToBeCommited.Enqueue(e);
			}
		}

		public bool Remove(string key)
		{
			var c = Get(key);
			if(c != null)
			{
				var e = EventToManageComponent<TComponent>.GetRemoveEvent(this, key);
				Engine.ChangesToBeCommited.Enqueue(e);
				return true;
			}
			return false;
		}

		public TComponent Get(string key)
		{
			if(components.ContainsKey(key))
			{
				return Particular.Dictionary.Get(components, key);
			}
			return null;
		}

		public void ImmediatelyAddComponent(TComponent component, string key)
		{
			Particular.Dictionary.Set(components, key, component);
			owner.Register(component);
		}

		public void ImmediatelyRemoveComponent(string key)
		{
			var component = Get(key);
			components.Remove(key);
			owner.Unregister(component);
		}
	}
}
