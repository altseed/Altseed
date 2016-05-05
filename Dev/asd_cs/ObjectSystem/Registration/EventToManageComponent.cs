using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal class EventToManageComponent<TOwner, TComponent> : IRegistrationEvent
		where TOwner : class 
		where TComponent : Component<TOwner>
	{
		public EventToManageComponent(
			ComponentManager<TOwner, TComponent> manager,
			TComponent component,
			string key,
			RegistrationCommand command)
		{
			Manager = manager;
			Component = component;
			Key = key;
			Command = command;
		}

		private ComponentManager<TOwner, TComponent> Manager { get; set; }
		private TComponent Component { get; set; }
		private string Key { get; set; }
		public RegistrationCommand Command { get; set; }

		public object Owner
		{
			get { return Manager; }
		}

		public object Content
		{
			get { return Component; }
		}

		public void Commit()
		{
			switch (Command)
			{
			case RegistrationCommand.Add:
				Manager.AddDirectly(Component, Key);
				break;

			case RegistrationCommand.Remove:
				Manager.RemoveDirectly(Key);
				break;
			}
		}

		public static EventToManageComponent<TOwner, TComponent> GetAddEvent(
			ComponentManager<TOwner, TComponent> manager,
			TComponent component,
			string key)
		{
			return new EventToManageComponent<TOwner, TComponent>(manager, component, key, RegistrationCommand.Add);
		}

		public static EventToManageComponent<TOwner, TComponent> GetRemoveEvent(
			ComponentManager<TOwner, TComponent> manager,
			string key)
		{
			return new EventToManageComponent<TOwner, TComponent>(manager, null, key, RegistrationCommand.Remove);
		} 
	}
}
