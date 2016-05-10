using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal class EventToManageComponent<TComponent> : ICommitable
		where TComponent : Component
	{
		public EventToManageComponent(
			IImmediateComponentmanager<TComponent> manager,
			TComponent component,
			string key,
			RegistrationCommand command)
		{
			Manager = manager;
			Component = component;
			Key = key;
			Command = command;
		}

		private IImmediateComponentmanager<TComponent> Manager { get; set; }
		private TComponent Component { get; set; }
		private string Key { get; set; }
		public RegistrationCommand Command { get; set; }

		public void Commit()
		{
			switch (Command)
			{
			case RegistrationCommand.Add:
				Manager.ImmediatelyAddComponent(Component, Key);
				break;

			case RegistrationCommand.Remove:
				Manager.ImmediatelyRemoveComponent(Key);
				break;
			}
		}

		public static EventToManageComponent<TComponent> GetAddEvent(
			ComponentManager<TComponent> manager,
			TComponent component,
			string key)
		{
			return new EventToManageComponent<TComponent>(manager, component, key, RegistrationCommand.Add);
		}

		public static EventToManageComponent<TComponent> GetRemoveEvent(
			ComponentManager<TComponent> manager,
			string key)
		{
			return new EventToManageComponent<TComponent>(manager, null, key, RegistrationCommand.Remove);
		} 
	}
}
