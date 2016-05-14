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
		public EventToManageComponent()
		{
		}

		private IImmediateComponentmanager<TComponent> Manager { get; set; }
		private TComponent Component { get; set; }
		private string Key { get; set; }
		private RegistrationCommand Command { get; set; }

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

		public void SetUpAsAddEvent(
			ComponentManager<TComponent> manager,
			TComponent component,
			string key)
		{
			Manager = manager;
			Component = component;
			Key = key;
			Command = RegistrationCommand.Add;
		}

		public void SetUpAsRemoveEvent(
			ComponentManager<TComponent> manager,
			string key)
		{
			Manager = manager;
			Key = key;
			Command = RegistrationCommand.Remove;
		} 
	}
}
